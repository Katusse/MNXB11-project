#include "extremedays.h"

#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>
#include <TH1D.h>
#include <TStyle.h>

#include <cstdlib>
#include <map>
#include <tuple>
#include <vector>
#include <iostream>
#include <limits>
#include <string>

// Accumulated measurements for one date.
struct DailyData {
    double sum = 0.0;
    int count = 0;
};

// Average temperature for one calendar day.
struct DailyAverage {
    int day_of_year;
    double temperature;
};

// Map month/day onto a calendar containing February 29.
// This makes the calendar indices consistent across years.
static int calendar_day(int month, int day)
{
    const int days_before_month[] = {
        0, 0, 31, 60, 91, 121, 152,
        182, 213, 244, 274, 305, 335
    };

    return days_before_month[month] + day;
}

// Analyze an already-loaded collection of measurements.
void analyze_temperature(
    const std::vector<Measurement>& measurements)
{
    if (measurements.empty()) {
        std::cerr << "No measurements available.\n";
        return;
    }

    // Key: year, month, day.
    std::map<std::tuple<int, int, int>, DailyData> daily_data;

    // 1. Accumulate measurements belonging to the same date.
    for (const auto& m : measurements) {
        auto date = std::make_tuple(
            m.get_year(),
            m.get_month(),
            m.get_day()
        );

        daily_data[date].sum += m.get_temperature();
        daily_data[date].count++;
    }

    // 2. Calculate one average per date and group by year.
    std::map<int, std::vector<DailyAverage>> yearly_data;

    for (const auto& entry : daily_data) {
        int year  = std::get<0>(entry.first);
        int month = std::get<1>(entry.first);
        int day   = std::get<2>(entry.first);

        const DailyData& data = entry.second;

        if (data.count == 0) {
            continue;
        }

        double average = data.sum / data.count;
        int doy = calendar_day(month, day);

        yearly_data[year].push_back({doy, average});
    }

    if (yearly_data.empty()) {
        std::cerr << "No daily averages available.\n";
        return;
    }

    // Frequency of each calendar day being an annual extreme.
    int hottest_frequency[367] = {};
    int coldest_frequency[367] = {};

    // 3. Find annual extremes and count their calendar days.
    for (const auto& year_entry : yearly_data) {
        const auto& days = year_entry.second;

        double hottest =
            -std::numeric_limits<double>::infinity();

        double coldest =
             std::numeric_limits<double>::infinity();

        // Find the maximum and minimum daily averages.
        for (const auto& d : days) {
            if (d.temperature > hottest) {
                hottest = d.temperature;
            }

            if (d.temperature < coldest) {
                coldest = d.temperature;
            }
        }

        // Count all dates tied for either extreme.
        for (const auto& d : days) {
            if (d.temperature == hottest) {
                hottest_frequency[d.day_of_year]++;
            }

            if (d.temperature == coldest) {
                coldest_frequency[d.day_of_year]++;
            }
        }
    }

    // 4. Plot hottest and coldest frequencies together.

    TH1D* hottest_hist = new TH1D(
        "hottest_hist",
        "Annual Temperature Extremes;Day of Year;Number of Years",
        366, 0.5, 366.5
    );

    TH1D* coldest_hist = new TH1D(
        "coldest_hist",
        "Annual Temperature Extremes;Day of Year;Number of Years",
        366, 0.5, 366.5
    );

    // Fill the histograms with the previously calculated frequencies.
    for (int d = 1; d <= 366; ++d) {
        hottest_hist->SetBinContent(
            d, hottest_frequency[d]
        );

        coldest_hist->SetBinContent(
            d, coldest_frequency[d]
        );
    }

    // Style the histograms.
    hottest_hist->SetFillColor(kRed);
    hottest_hist->SetLineColor(kRed);
    hottest_hist->SetBarWidth(0.45);
    hottest_hist->SetBarOffset(0.05);

    coldest_hist->SetFillColor(kBlue);
    coldest_hist->SetLineColor(kBlue);
    coldest_hist->SetBarWidth(0.45);
    coldest_hist->SetBarOffset(0.50);

    // Create one canvas for both histograms.
    TCanvas* c1 = new TCanvas(
        "extremes_canvas",
        "Hottest and Coldest Days",
        1400, 700
    );

    // Set the display range and axis divisions.
    hottest_hist->GetXaxis()->SetRangeUser(1, 366);
    hottest_hist->GetXaxis()->SetNdivisions(12);

    // Removes an Unnecessary Legend
    gStyle->SetOptStat(0);

    // Draw both sets of bars on the same graph.
    hottest_hist->Draw("HIST BAR");
    coldest_hist->Draw("HIST BAR SAME");

    c1->SetGrid();
    c1->SaveAs("temperature_extremes.png");

}
// Load data using the existing loader, then run the analysis.
void analyze_temperature(const std::string& filename) {
    std::vector<Measurement> measurements =
        read_measurements(filename);

    std::cout << "Read " << measurements.size()
              << " valid measurements.\n";

    analyze_temperature(measurements);
}

