
#include "load_data.h"

#include <iostream>
#include <vector>


//This run_weather.C is a macro that uses the weather measremetns in read_measuremnts 
//It writes a file weather_data.root, where the new tree is stored with
// all branches
//run  this by going into the root apptainer and running
//.x run_weather.C+ the weather file is stored in
//right now under the main MNXB11-project directory

void run_weather()
{
    // Read the cleaned weather data
    std::vector<Measurement> measurements =
        read_measurements(
            "datasets/clean_smhi-opendata_1_53430_20261007_164857_Lund.csv"
        );

    std::cout << "Read " << measurements.size()
              << " valid measurements." << std::endl;

    // Save the measurements in a ROOT file
    persist_measurements(measurements, "weather_data.root");
}