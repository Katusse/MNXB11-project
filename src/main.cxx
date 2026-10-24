#include <iostream>
#include <vector>
#include "load_data.h"


//compile with g++ src/main.cxx src/load_data.cxx -o read_weather \
    $(root-config --cflags --libs)
//and then run with ./read_weather datasets/filename.csv

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0]
                  << " <input_filename>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    try {
        std::vector<Measurement> measurements =
            read_measurements(filename);

        std::cout << "Read " << measurements.size()
                  << " valid measurements." << std::endl;

        persist_measurements(measurements, "weather_data.root");

        std::cout << "Measurements saved." << std::endl;
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << std::endl;
        return 1;
    }

    return 0;
}