#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TLine.h>
#include <TLegend.h>
#include <iostream>
#include <vector>
#include <map>
#include <utility>

//code that produces 3 plots looking at extreme temperatures over the years.
//it loads the tree from weather_data.root, created by run_weather.C
//yearly and monthly extremes are extracted and plotted.
//plot 1: plots each year's hottest and coldest temp with a line
//connecting them. the points are colored by the month they occured in
//plot 2: plots the difference between the hottest and coldest temps 
//for each year as a single line without any points
//plot 3: same as 2 but for each month, so 12x as many points.
//the plots are saved as .png's in a results folder /MNXB11-project/results
//the code is run by typing the below in the root terminal from the MNXB11-project folder
// root [] .x yearly_extremes.C

struct YearExtremes {
    double min_temp = 9999.0;
    int min_month = -1;
    double max_temp = -9999.0;
    int max_month = -1;
};

struct MonthExtremes {
    double min_temp = 9999.0;
    double max_temp = -9999.0;
};

void yearly_extremes() {
    // open the root file and get tree
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

    Int_t year, month;
    Double_t temperature;
    tree->SetBranchAddress("year", &year);
    tree->SetBranchAddress("month", &month);
    tree->SetBranchAddress("temperature", &temperature);

    // tracking extremes per year and per (year, month)
    std::map<int, YearExtremes> yearly_data;
    std::map<std::pair<int, int>, MonthExtremes> monthly_data;

    Long64_t nentries = tree->GetEntries();
    for (Long64_t i = 0; i < nentries; ++i) {
        tree->GetEntry(i);

        // filter out unphysical or invalid values
        if (temperature < -80.0 || temperature > 60.0) continue;

        // yearly extremes
        YearExtremes &y_ext = yearly_data[year];
        if (temperature < y_ext.min_temp) {
            y_ext.min_temp = temperature;
            y_ext.min_month = month;
        }
        if (temperature > y_ext.max_temp) {
            y_ext.max_temp = temperature;
            y_ext.max_month = month;
        }

        // monthly extremes
        MonthExtremes &m_ext = monthly_data[{year, month}];
        if (temperature < m_ext.min_temp) m_ext.min_temp = temperature;
        if (temperature > m_ext.max_temp) m_ext.max_temp = temperature;
    }

    if (yearly_data.empty()) {
        std::cerr << "Error: No valid entries found." << std::endl;
        return;
    }

    // plot 1: yearly extreme temparatures with colored points depending on the month
    Color_t month_colors[12] = {
        kBlue, kCyan + 1, kTeal + 2, kGreen + 2, kSpring + 4, kYellow + 2,
        kOrange + 7, kRed + 1, kPink + 7, kMagenta, kViolet + 1, kAzure - 4
    };
    const char* month_names[12] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    TGraph *gr_month[12];
    for (int m = 0; m < 12; ++m) {
        gr_month[m] = new TGraph();
        gr_month[m]->SetMarkerStyle(20);
        gr_month[m]->SetMarkerSize(1.0);
        gr_month[m]->SetMarkerColor(month_colors[m]);
    }

    int first_year = yearly_data.begin()->first;
    int last_year = yearly_data.rbegin()->first;
    double global_min = 1e9, global_max = -1e9;

    for (const auto &kv : yearly_data) {
        int yr = kv.first;
        const YearExtremes &ext = kv.second;
        if (ext.min_temp < global_min) global_min = ext.min_temp;
        if (ext.max_temp > global_max) global_max = ext.max_temp;

        if (ext.min_month >= 1 && ext.min_month <= 12)
            gr_month[ext.min_month - 1]->SetPoint(gr_month[ext.min_month - 1]->GetN(), yr, ext.min_temp);
        if (ext.max_month >= 1 && ext.max_month <= 12)
            gr_month[ext.max_month - 1]->SetPoint(gr_month[ext.max_month - 1]->GetN(), yr, ext.max_temp);
    }

    TCanvas *c1 = new TCanvas("c1", "Yearly Extreme Temperatures", 1100, 650);
    c1->SetGridx();
    c1->SetGridy();
    c1->SetRightMargin(0.16);

    TH1F *frame1 = c1->DrawFrame(first_year - 1, global_min - 5.0, last_year + 1, global_max + 5.0);
    frame1->SetTitle("Yearly Extreme Temperatures;Year;Temperature (#circC)");

    for (const auto &kv : yearly_data) {
        TLine *line = new TLine(kv.first, kv.second.min_temp, kv.first, kv.second.max_temp);
        line->SetLineColor(kGray + 2);
        line->SetLineWidth(2);
        line->Draw("SAME");
    }

    for (int m = 0; m < 12; ++m) {
        if (gr_month[m]->GetN() > 0) gr_month[m]->Draw("P SAME");
    }

    TLegend *leg = new TLegend(0.85, 0.15, 0.98, 0.88);
    leg->SetHeader("Month", "C");
    for (int m = 0; m < 12; ++m) {
        if (gr_month[m]->GetN() > 0) leg->AddEntry(gr_month[m], month_names[m], "p");
    }
    leg->Draw();
    c1->SaveAs("results/yearly_extreme_temperatures.png");

    // plot 2: extreme temperature difference over the years
    TGraph *gr_year_diff = new TGraph();
    int pt_yr = 0;
    for (const auto &kv : yearly_data) {
        double diff = kv.second.max_temp - kv.second.min_temp;
        gr_year_diff->SetPoint(pt_yr++, kv.first, diff);
    }

    gr_year_diff->SetLineColor(kBlack);
    gr_year_diff->SetLineWidth(2);
    gr_year_diff->SetTitle("Yearly Extreme Temperature Difference;Year;#DeltaT (#circC)");

    TCanvas *c2 = new TCanvas("c2", "Yearly Difference", 900, 600);
    c2->SetGridx();
    c2->SetGridy();

   // AL draws only a line without scatter points
    gr_year_diff->Draw();
    c2->SaveAs("results/yearly_difference.png");

   // plot 3: extreme temperature difference over the years for each month
    TGraph *gr_month_diff = new TGraph();
    int pt_mo = 0;
    for (const auto &kv : monthly_data) {
        int yr = kv.first.first;
        int mo = kv.first.second;
        double diff = kv.second.max_temp - kv.second.min_temp;

        double time_x = yr + (mo - 0.5) / 12.0;
        gr_month_diff->SetPoint(pt_mo++, time_x, diff);
    }

    gr_month_diff->SetLineColor(kBlue + 2);
    gr_month_diff->SetLineWidth(2);
    gr_month_diff->SetTitle("Monthly Extreme Temperature Difference Over the Years;Year;#DeltaT (#circC)");

    TCanvas *c3 = new TCanvas("c3", "Monthly Difference Over Time", 1000, 600);
    c3->SetGridx();
    c3->SetGridy();

    gr_month_diff->Draw("AL");
    c3->SaveAs("results/monthly_difference_over_years.png");
}