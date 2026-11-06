#include "run_Love_analysis.h"
#include "run_weather.h"
#include "yearly_average.h"
#include "yearly_extremes.h"
#include "daily_average.h"
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
    // Use Lund by default if no city is supplied.
    const char* city = "lund";

    if (argc > 2) {
        std::cerr << "Usage: " << argv[0]
                  << " [city]\n";
        return 1;
    }

    if (argc == 2) {
        city = argv[1];
    }

    std::filesystem::create_directories("results");

    run_weather(city);
    yearly_average();
    run_Love_analysis(city);
    yearly_extremes();
    daily_average();

    std::remove("weather_data.root");
    

    return 0;
}
