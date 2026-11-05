#pragma once
#include <vector>
#include <TTree.h>

void yearly_average();

bool valid_measurement_year(
    const std::vector<Double_t>& count_per_month,
    const Long64_t i
);