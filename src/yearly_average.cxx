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
#include <numeric>  //provides accumulate
#include <TStyle.h>

//This file is about my homework
//Here I open the tree from weather_data.root, created by runnign run_weather.C
//Then the yearly averages are calculated by the mean od measurements
//A histogram is plotted with the overall mean 'mean_T'
//and the yearly means 'yearly_mean_T'
//the png is called ' yearly_average_temperature.png' and stored in results directory
//run the code in root from MNXB11-project by
// .x yearly_average.C+
//===============================================================================================

void yearly_average(){

    //Access tree in weather_data.root in main directory, the file is created by run_weather.cxx
    TFile *file= TFile::Open("weather_data.root", "READ");
    //open tree from; MNXB11 Temperature file, from weather_data.root. This contains the year, month and temperature data
    TTree *tree = (TTree*)file->Get("MNXB11_Temperature");
    
    //Analyse year and temperature, variables used to store the data from the tree
    Int_t year; 
    Int_t month;
    Double_t temperature;
    //accessing the branches for year and temperature
    tree->SetBranchAddress("year", &year);
    tree->SetBranchAddress("month", &month);
    tree->SetBranchAddress("temperature", &temperature);

    //Data stored in 3 vectors Years, Months and Temperatures
    std::vector<int> Years{};
    std::vector<double> Temperatures{};
    std::vector<int> Months{};



    for (Long64_t i = 0; i < tree->GetEntries(); i++) {
        tree->GetEntry(i);

        Years.push_back(year);
        Temperatures.push_back(temperature);
        Months.push_back(month);
    }

    if (Years.empty()) {
        std::cerr << "Error: No measurements were loaded.\n";
        return;
    }
    //Data is loaded
    //=============================================================================================================
    //Now we want to find the mean temperature per year, and the overall mean temperature

    //A list with all years , once, between the measurements, even if in that year was nothing measured
    //used later to find years were measurements are missing
    std::vector<Long64_t> measurement_period;
    Long64_t first_year = *std::min_element(Years.begin(), Years.end()); //min_element returns iterator, need to dereference with * to get smallest value 
    Long64_t last_year = *std::max_element(Years.begin(), Years.end());

    //fill measurement_period with all years between first and last year
    for (Long64_t i=first_year; i<=last_year; i++){
        measurement_period.push_back(i);
    }

    //Find all the temperatures of 1 year, calculate the mean and
    // then do it for the next year
    //The  yearly mean temperature is stored in mean T 
    //while all measurements years are in meas_years
    std::vector<Double_t> yearly_mean_T{};
    std::vector<Long64_t> meas_years{};

    std::vector<Long64_t> not_recorded_years{};


        // for loop over all years in the measurement period, to find the mean temperature per year, i is the year
        for (Long64_t i : measurement_period){

            //vector to store the temperatures of 1 year, used to calculate the mean temperature of that year
            std::vector<Double_t> T_in_year{};

            
            
            // get the tempearatures for that specific year
            Double_t sum(0);
            Long64_t count(0);
            
            std::vector<Double_t> count_per_month(12,0); //vector with all the measuremetn point per month of 1 year, uses double bc of calc later

            // for loop over all measurements, masks all the measurements of the year i, and stores them in T_in_year, 
            // k is the index of the measurement in the vector
            for (std::size_t k=0; k<Years.size(); k++){
                if (Years[k]==i) {
                    T_in_year.push_back(Temperatures[k]);
                    sum += Temperatures[k];
                    count++;

                    //count measutremetns per month
                    //takes the month of measurement k, and adds 1 to the corresponding month in count_per_month
                    count_per_month[Months[k]-1]++; //Months[k]-1 because the vector starts at 0, but months start at 1
                }
            //Now we have all the temperatures of the year i in T_in_year, and the sum and count of the measurements
            }



            //check if there are measurements during the year and if the year has equal distribution of measurements points
            //if yes then calculate the mean temperature of that year and add it to yearly_mean_T, 
            //if not add the year to not_recorded_years
            if (count>0 && valid_measurement_year(count_per_month, i)){ 
                //add the  validyear to the list of years with measurements
                meas_years.push_back(i);
                yearly_mean_T.push_back( sum/count);
            }
            else {
                not_recorded_years.push_back(i);
                std::cout<<"The year "<< i << "was not recorded.\n" ;
            }
        }

    // Now we have the mean temperature of every year in yearly_mean_T, and the years with measurements in meas_years
    //######################################################################
    
    // get the overall mean temperature from the means of every year yearly_mean_T
    Double_t sum(0);
    for (Double_t temperatures: yearly_mean_T){
            sum += temperatures;  
        }
    Double_t mean_T=sum/yearly_mean_T.size();


    //get the running average over a period of 21 years/ -10 and +10
    //include also years with no measurments
    std::vector<Double_t> running_averages{};
    std::vector<Double_t> running_years{};

    // for loop over all years in measurement period, center_year is the year in the middle of the 21 year period
    for (Long64_t center_year : measurement_period) {

        Double_t sum = 0.0;
        Long64_t count = 0;

        // Consider all years with measurements
        for (std::size_t i = 0; i < meas_years.size(); i++) {

            // Include years within 10 years before or after center_year
            //If one year was not recorded it will be skipped, and count will not count that year
            if (meas_years[i] >= center_year - 10 &&
                meas_years[i] <= center_year + 10) {

                sum += yearly_mean_T[i];
                count++;
            }
        }

        // Calculate the average if at least one year has measurements
        if (count > 0) {
            running_averages.push_back(sum / count);
            running_years.push_back(center_year);
        }
    // running average stored in running_averages, and the corresponding years in running_years
    }





    //######################################################################
    //plotting

    // Create one bin for each year
    Long64_t number_of_years = last_year - first_year + 1;

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

    TH1D *no_record = new TH1D(
        "no_record",
        "Mean temperature per year;Year;Mean temperature (#circC)",
        number_of_years,
        first_year - 0.5,
        last_year + 0.5
    );


    //Creating nice plots, start by finding the size of the plot
    // Determine a suitable vertical range for the plot
    Double_t plot_min = *std::min_element(
        yearly_mean_T.begin(), yearly_mean_T.end()
    );

    Double_t plot_max = *std::max_element(
        yearly_mean_T.begin(), yearly_mean_T.end()
    );

    // Add some space above and below the data
    Double_t margin = 0.1 * (plot_max - plot_min);

    plot_min -= margin;
    plot_max += margin;

    above->SetMinimum(plot_min);
    above->SetMaximum(plot_max);

    below->SetMinimum(plot_min);
    below->SetMaximum(plot_max);

    white->SetMinimum(plot_min);
    white->SetMaximum(plot_max);

    no_record->SetMinimum(plot_min);
    no_record->SetMaximum(plot_max);

    // Fill each year's bin with its mean temperature
    for (std::size_t i = 0; i < meas_years.size(); i++) {

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

    // fill not bins with not recorded years with max value of plot
    for (Long64_t year: not_recorded_years){
            int bin = year-first_year+1;
            no_record->SetBinContent(bin, plot_max);
        }


    above->SetLineColor(kRed);
    above->SetFillColor(kRed);
    below->SetLineColor(kBlue);
    below->SetFillColor(kBlue);
    white->SetFillColor(10);
    white->SetLineColor(10);
    no_record->SetLineColor(kGray);
    no_record->SetFillColor(kGray);
    no_record->SetFillStyle(3003);




    // Draw the overall average as a dashed line
    TLine *average_line = new TLine(
        first_year - 0.5, mean_T,
        last_year + 0.5, mean_T
    );

    average_line->SetLineStyle(2);
    average_line->SetLineColor(kBlack);
    average_line->SetLineWidth(2);

    // PLotting the running average
    TGraph *running_average = new TGraph(
        running_averages.size(),
        running_years.data(),
        running_averages.data()
    );

    running_average->SetLineStyle(1);
    running_average->SetLineColor(kBlack);
    running_average->SetLineWidth(3);

    TCanvas *c = new TCanvas(
        "canvas", "Yearly average temperature", 900, 600
    );
    c->SetFillColor(kWhite);

    // Set the style to not show the statistics box
    gStyle->SetOptStat(0);

    above->Draw("HIST");
    below->Draw("HIST SAME");
    white->Draw("HIST SAME");
    no_record->Draw("HIST SAME"); //plot histogram on top of the existing plots

    average_line->Draw("SAME");
    running_average->Draw("L SAME"); //graph a line ontop of the existing plots
    c->RedrawAxis();


    // Save the canvas as a PNG file in the results directory
    c->SaveAs("results/yearly_average_temperature.png");
}


//############################################################
//function definition

// takes in a vector will all counts per month of 1 year (i) and year i and checks if the years has equally distributed measurements

bool valid_measurement_year(const std::vector<Double_t>& count_per_month, const Long64_t i) { //i is the year

    Double_t max_count=*std::max_element(count_per_month.begin(), count_per_month.end());
    Double_t min_count=*std::min_element(count_per_month.begin(), count_per_month.end());

    // Check if the difference between max and min count is less than 1/4 of the max count
    if (max_count- min_count< (max_count/4) ){
        return true;
    } // Check if the difference is between 1/4 and 1/3 of the max count
    else if ((max_count- min_count> (max_count/4)) &&  (max_count- min_count< (max_count/3))){
        std::cout<<"In year "<< i<< " there is a large difference between the measurement points taken in each month. The year is still included. \n ";
        return true;
    }else // Check if the difference is greater than 1/3 of the max count and reject the year
    {
        std::cout<<"In year "<< i<< " there is a large difference between the measurement points taken in each month. The year is not included. \n ";
        return false;
    }
    
}
