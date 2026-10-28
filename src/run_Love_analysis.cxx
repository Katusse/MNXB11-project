#include "run_Love_analysis.h"
#include "dataset_registry.h"
#include "extremedays.h"
#include <exception>
#include <iostream>
#include <string>

void run_Love_analysis(const char* dataset_name)
{
    try {
        std::string filename = get_dataset_path(dataset_name);

        std::cout << "Analyzing dataset: "
                  << dataset_name << '\n';

        std::cout << "File: "
                  << filename << '\n';

        // Load the data and perform the analysis.
        analyze_temperature(filename);
    }
    catch (const std::exception& e) {
        std::cerr << "Analysis failed: "
                  << e.what() << '\n';
    }
}