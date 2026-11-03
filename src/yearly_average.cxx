#include "yearly_average.h"
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <TH1D.h>
#include <TLine.h>

//This file is about my homework
//Here I open the tree from weather_data.root, created by runnign run_weather.C
//Then the yearly averages are calculated by the mean od measurements
//A histogram is plotted with the overall mean 'mean_T'
//and the yearly means 'yearly_mean_T'
//the png is called ' yearly_average_temperature.png' and stored in main diectory
//run the code in root from MNXB11-project by
// .x yearly_average.C+

void yearly_average(){

    //Open data from tree in weather_data.root
    TFile *file= TFile::Open("weather_data.root", "READ");
    //open tree from the MNXB11 Temperature file
    TTree *tree = (TTree*)file->Get("MNXB11_Temperature");
    
    //Analyse year and temperature
    Int_t year; 
    Double_t temperature;
    //accessing the branches for year and temperature
    tree->SetBranchAddress("year", &year);
    tree->SetBranchAddress("temperature", &temperature);

    //Data stored in 2 vectors Years and Temperatures
    std::vector<int> Years{};
    std::vector<double> Temperatures{};

    for (Long64_t i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);

        Years.push_back(year);
        Temperatures.push_back(temperature);
    }

//Find all the temperatures of 1 year, calculate the mean and
// then do it for the next year
//The  mean temperature is stored in mean T 
//while all measrement years are in meas_years
std::vector<double> yearly_mean_T{};
std::vector<Double_t> meas_years{};

    //create an array with all years with measurements only once
    for (std::size_t i = 0; i < Years.size(); i++){

        // find one entry and only put it in meas_years if it hasnt been in there before
        if (std::find(meas_years.begin(), meas_years.end(), Years[i])
            == meas_years.end())
        {
            meas_years.push_back(Years[i]);
        }   
    }

    // create a mask so that we consider one year (i) at a time
    for (Long64_t i : meas_years){
        std::vector<Double_t> T_in_year{};
        std::vector<bool> mask{};

        // create a mask for a specific year i
        for (Long64_t year : Years){
            mask.push_back(year==i);
        }

        
        
        // get the tempearatures for that specific year
        Double_t sum(0);
        for (std::size_t k=0; k<Temperatures.size(); k++){
            if (mask[k]) {
                T_in_year.push_back(Temperatures[k]);
                sum += Temperatures[k];
            }
        }
        yearly_mean_T.push_back( sum/T_in_year.size());
    }

    // get the overall mean temperature from the means of every year yearly_mean_T
Double_t sum(0);
for (Double_t temperatures: yearly_mean_T){
        sum += temperatures;  
    }
Double_t mean_T=sum/yearly_mean_T.size();


//######################################################################
//plotting

// Create one bin for each year
int first_year = meas_years.front();
int last_year = meas_years.back();
int number_of_years = last_year - first_year + 1;

TH1D *above = new TH1D(
    "above",
    "Mean temperature per year;Year;Mean temperature (#circC)",
    number_of_years,
    first_year - 0.5,
    last_year + 0.5
);

TH1D *below = new TH1D(
    "below",
    "Mean temperature per year;Year;Mean temperature (#circC)",
    number_of_years,
    first_year - 0.5,
    last_year + 0.5
);

TH1D *white = new TH1D(
    "white",
    "Mean temperature per year;Year;Mean temperature (#circC)",
    number_of_years,
    first_year - 0.5,
    last_year + 0.5
);

// Fill each year's bin with its mean temperature
for (std::size_t i = 0; i < meas_years.size(); i++) {
    //histogram->SetBinContent(
        //static_cast<int>(meas_years[i] - first_year + 1),
        //yearly_mean_T[i]
    //);
    int bin = static_cast<int>(meas_years[i] - first_year + 1);
    
    //Double_t difference =yearly_mean_T[i]- mean_T;



    if (yearly_mean_T[i]>=mean_T){
        above->SetBinContent(bin, yearly_mean_T[i]);
        white->SetBinContent(bin, mean_T);
    }
    else {
        below->SetBinContent(bin, mean_T);
        white->SetBinContent(bin, yearly_mean_T[i]);
    }
}

above->SetLineColor(kRed);
above->SetFillColor(kRed);
below->SetLineColor(kBlue);
below->SetFillColor(kBlue);
white->SetFillColor(10);
white->SetLineColor(10);




// Draw the overall average as a dashed line
TLine *average_line = new TLine(
    first_year - 0.5, mean_T,
    last_year + 0.5, mean_T
);

TCanvas *c = new TCanvas(
    "canvas", "Yearly average temperature", 900, 600
);
c->SetFillColor(kWhite);

above->Draw("HIST");
below->Draw("HIST SAME");
white->Draw("HIST SAME");

average_line->Draw("SAME");
c->RedrawAxis();



c->SaveAs("results/yearly_average_temperature.png");
}
