#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>
#include <map>
#include <cmath>

// Code that produces Figure 2: Histogram showing the mean temperature on each day of the year.
// It loads the tree from weather_data.root.
// The mean temperatures are averageed across all years over each of the doy (day of the year) are plotted.
// The standard deviation is plotted as error bard as in Figure 2.
// The plot is then saved as 'daily_average_temperature.png' in the project directory.
// Run the code in ROOT from the MNXB11-project directory and then:
// root [] .x src/daily_average.C+

int get_day_of_year(int year, int month, int day) {
    const int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    bool leap = ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
    int doy = day;
    for (int m = 1; m < month; ++m) {
        doy += days_in_month[m];
        if (m == 2 && leap) doy += 1;
    }
    return doy;
}

void daily_average() {
    TFile *file = TFile::Open("weather_data.root", "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Error: Cannot open weather_data.root" << std::endl;
        return;
    }

    TTree *tree = (TTree*)file->Get("MNXB11_Temperature");
    if (!tree) {
        std::cerr << "Error: Cannot find TTree 'MNXB11_Temperature'" << std::endl;
        return;
    }

    Int_t year, month, day;
    Double_t temperature;
    
    tree->SetBranchAddress("year", &year);
    tree->SetBranchAddress("month", &month);
    tree->SetBranchAddress("day", &day);
    tree->SetBranchAddress("temperature", &temperature);

    // To use for std::map and to use it for doy for the plot
    struct DateKey {
        int year;
        int doy;
        bool operator<(const DateKey& other) const {
            if (year != other.year) return year < other.year;
            return doy < other.doy;
        }
    };

    std::map<DateKey, std::vector<double>> daily_readings;

    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; i++) {
        tree->GetEntry(i);

        if (temperature < -80.0 || temperature > 60.0) continue;

        int doy = get_day_of_year(year, month, day);
        if (doy >= 1 && doy <= 366) {
            daily_readings[{year, doy}].push_back(temperature);
        }
    }
    
    if (daily_readings.empty()) {
        std::cerr << "Error: No valid entries found." << std::endl;
        return;
    }

    // Mean temperature for each doy over all the years
    std::vector<std::vector<double>> daily_means_per_doy(367);
    for (const auto &readings_i : daily_readings) {
        const auto &readings = readings_i.second;
        if (readings.empty()) continue;

        double sum = 0.0;
        for (double t : readings) {
            sum += t;
        }
        double daily_mean = sum / readings.size();
        int doy = readings_i.first.doy;
        daily_means_per_doy[doy].push_back(daily_mean);
    }

    // Plotting
    TH1D *h_1 = new TH1D(
        "h_1",
        "Mean temperature on each day of the year;Day of year;Temperature [#circC]",
        366, 0.5, 366.5
    );

    h_1->SetFillColor(kBlue - 8);
    h_1->SetLineColor(kBlue + 1);
    h_1->SetLineWidth(2);

    // Calculating overall mean and standard deviation for each doy for all the years
    for (int doy_i = 1; doy_i <= 366; doy_i++) {
        const auto &means_list = daily_means_per_doy[doy_i];
        if (means_list.empty()) continue;

        double sum = 0.0;
        for (double m : means_list) sum += m;
        double overall_mean = sum / means_list.size();

        double sqr_sum = 0.0;
        for (double m : means_list) {
            sqr_sum += (m - overall_mean) * (m - overall_mean);
        }
        double stdev = std::sqrt(sqr_sum / means_list.size());

        h_1->SetBinContent(doy_i, overall_mean);
        h_1->SetBinError(doy_i, stdev);
    }

    TCanvas *c = new TCanvas("canvas", "Mean temperature on each day of the year.", 1000, 600);
    c->SetFillColor(kWhite);
    h_1->Draw("E2");
    c->RedrawAxis();
    c->SaveAs("daily_average_temperature.png");
}