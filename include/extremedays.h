#pragma once

#include <string>
#include <vector>
#include "load_data.h"

void analyze_temperature(
    const std::vector<Measurement>& measurements
);

void analyze_temperature(
    const std::string& filename
);