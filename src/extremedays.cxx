#include "extremedays.h"

#include <TCanvas.h>
#include <TGraph.h>
#include <TAxis.h>

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

// Convert a calendar-day index into a readable month/day label.
static std::string calendar_label(int doy)
{
    const int days_in_month[] = {
        31, 29, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    const char* months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    int day = doy;

    for (int month = 0; month < 12; ++month) {
        if (day <= days_in_month[month]) {
            return std::string(months[month]) + " " +
                   std::to_string(day);
        }

        day -= days_in_month[month];
    }

    return "";
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

    // 4. Print a summary.
    std::cout << "Years represented: "
              << yearly_data.size() << '\n';

    std::cout << "Day of year | Calendar date"
              << " | Hottest frequency"
              << " | Coldest frequency\n";

    for (int d = 1; d <= 366; ++d) {
        if (hottest_frequency[d] > 0 ||
            coldest_frequency[d] > 0) {

            std::cout << d << " | "
                      << calendar_label(d) << " | "
                      << hottest_frequency[d] << " | "
                      << coldest_frequency[d] << '\n';
        }
    }

    // 5. Plot the hottest-day frequencies.
    TGraph* hottest_graph = new TGraph();

    for (int d = 1; d <= 366; ++d) {
        hottest_graph->SetPoint(
            d - 1, d, hottest_frequency[d]
        );
    }

    TCanvas* c1 = new TCanvas(
        "hottest_canvas", "Hottest Days", 1200, 600
    );

    hottest_graph->SetTitle(
        "Frequency of Annual Hottest Days;"
        "Calendar Date;Number of Years"
    );

    hottest_graph->SetMarkerStyle(20);
    hottest_graph->SetMarkerColor(kRed);
    hottest_graph->SetLineColor(kRed);
    hottest_graph->GetXaxis()->SetLimits(1, 366);
    hottest_graph->GetXaxis()->SetNdivisions(12);
    hottest_graph->Draw("ALP");

    c1->SetGrid();
    c1->SaveAs("hottest_days.png");

    // 6. Plot the coldest-day frequencies.
    TGraph* coldest_graph = new TGraph();

    for (int d = 1; d <= 366; ++d) {
        coldest_graph->SetPoint(
            d - 1, d, coldest_frequency[d]
        );
    }

    TCanvas* c2 = new TCanvas(
        "coldest_canvas", "Coldest Days", 1200, 600
    );

    coldest_graph->SetTitle(
        "Frequency of Annual Coldest Days;"
        "Calendar Date;Number of Years"
    );

    coldest_graph->SetMarkerStyle(20);
    coldest_graph->SetMarkerColor(kBlue);
    coldest_graph->SetLineColor(kBlue);
    coldest_graph->GetXaxis()->SetLimits(1, 366);
    coldest_graph->GetXaxis()->SetNdivisions(12);
    coldest_graph->Draw("ALP");

    c2->SetGrid();
    c2->SaveAs("coldest_days.png");
}

// Load data using the existing loader, then run the analysis.
void analyze_temperature(const std::string& filename)
{
    std::vector<Measurement> measurements =
        read_measurements(filename);

    std::cout << "Read " << measurements.size()
              << " valid measurements.\n";

    analyze_temperature(measurements);
}
