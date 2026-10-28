#include "run_Love_analysis.h"

#include <iostream>


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

    run_Love_analysis(city);

    return 0;
}