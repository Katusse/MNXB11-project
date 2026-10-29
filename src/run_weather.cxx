
#include "run_weather.h"
#include "load_data.h"
#include "dataset_registry.h"

#include <exception>
#include <iostream>
#include <string>
#include <vector>

void run_weather(const char* dataset_name)
{
    try {
        std::string filename = get_dataset_path(dataset_name);

        std::cout << "Loading dataset: "
                  << dataset_name << '\n';

        std::cout << "File: "
                  << filename << '\n';

        // Read the cleaned weather data
        std::vector<Measurement> measurements =
            read_measurements(filename);

        std::cout << "Read " << measurements.size()
                  << " valid measurements." << std::endl;

        // Save the measurements in a ROOT file
        persist_measurements(measurements, "weather_data.root");
    }
    catch (const std::exception& e) {
        std::cerr << "Weather data loading failed: "
                  << e.what() << '\n';
    }
}
