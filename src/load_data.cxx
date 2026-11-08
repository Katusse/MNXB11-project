#include <iostream>
#include <string>
#include <vector>
#include "load_data.h"
#include <stdexcept>
#include <fstream>
#include <TTree.h>
#include <TFile.h>



//This file is defining the functions from the header file load_data.h

//use and implent ROOT (ClassImp) to include Measurement class in ROOT
ClassImp(Measurement);


//Define the constructor that defines one row
Measurement:: Measurement(int year, int month, int day,
                int hour, int minute, int second,
                double temperature, char grading)
                    : year_(year),
                    month_(month),
                    day_(day),
                    hour_(hour),
                    minute_(minute),
                    second_(second),
                    temperature_(temperature),
                    grading_(grading)
                    {}

//Define the default contructor by setting every number to 0 and the character to empty?
Measurement:: Measurement()
                    : year_(0),
                    month_(0),
                    day_(0),
                    hour_(0),
                    minute_(0),
                    second_(0),
                    temperature_(0.0),
                    grading_('\0')
                    {}


//Define the getters


int Measurement::get_year() const {
    return year_;
}

int Measurement::get_month() const {
    return month_;
}

int Measurement::get_day() const {
    return day_;
}

int Measurement::get_hour() const {
    return hour_;
}

int Measurement::get_minute() const {
    return minute_;
}

int Measurement::get_second() const {
    return second_;
}

double Measurement::get_temperature() const {
    return temperature_;
}

char Measurement::get_grading() const {
    return grading_;
}


//Define function is_valid to decide which obejecst have the correct date format
bool Measurement::is_valid()const{

    bool timeline_valid=((year_>1500) &&(year_<= 2026));

    //Check the months of the year are less than 13
    bool year_valid=((month_>= 1) && (month_<=12));

    //Define a function that checks the corrects days in the month, 
    //use 28 days for february and account for leap days later
    const std::vector <int> days_in_month={31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int max_day = days_in_month[month_ -1]; //indexing srts at 0
    //find leap years as leap years divide by 4 or 400 but dont happen at centuries and  
    bool leap_year=((year_%4==0) && (year_ % 100 != 0))
        || year_ % 400 == 0;
    if (month_ == 2 && leap_year) {
        max_day = 29;
    }
    // define which months are valid, the months that have the correct number of days
    bool month_valid=((day_ >= 1) && (day_<= max_day));
    

    //Check the time data
    bool day_valid=(((hour_ >= 0) && (hour_<24)) && ((minute_ >= 0) && (minute_<60)) && ((second_ >= 0) && (second_<60)));

    //If all the above was checked the date is valid and it returns true
    bool date_valid=(day_valid && month_valid && year_valid && timeline_valid);

    //returns True if data is valid
    return date_valid;
}

//###########################################

//function that opens a file and reads the data to a vector of measurements, returns the vector
// & refers to filename, we dont copy it
std::vector<Measurement> read_measurements(const std::string& filename){
    std::ifstream file{filename}; //input file stream, opens the file for reading, extracts data from file

    if (!file.is_open()){
        throw std::runtime_error("Could not open file:" + filename);
    }


  std::string date, time;
  double temperature;
  char grading;


  //The data that we use later is stored in measurements
  std::vector<Measurement> measurements;

//separate data from ifstream, >> extracts data, sep. by whitespaces and stores it in variable
//date in format YYYY-MM-DD, time in format HH:MM:SS, temperature in float, grading in char
while (file >> date >> time >> temperature >> grading){
    // convert the dates and times into integers (stoi=string to integer)
    int year = std::stoi(date.substr(0,4));
    int month= std::stoi(date.substr(5,2));
    int day= std::stoi(date.substr(8,2));
    int minute = std::stoi(time.substr(3, 2));
    int second = std::stoi(time.substr(6, 2));
    int hour= std::stoi(time.substr(0,2));

    //define one object called measurement which is in the class/ one row
    Measurement measurement{
    year, month, day,
    hour, minute, second,
    temperature, grading
    };
    //check if the measurement has the valid format and type, decide if we eant to use it
    if (measurement.is_valid()){
        measurements.push_back(measurement);
    }
    else {//warn but dont stop program (no throw)
        std::cerr <<"Warning:Skippping invalid measurement from date, time: "<<
        year << "-" << month<<"-" << day << " " << hour<< ":" << minute << ":" << second<< std::endl;
    }
    }

return measurements;
}

void persist_measurements(const std::vector<Measurement>& measurements,
                          const std::string& output_filename) {
  // Note: We need .c_str() because TFile expects a C-string (char*) but
  // input_filename is a std::string, need a C_style string for Root, becomes char*
  TFile output_file{output_filename.c_str(), "RECREATE"};
  
  if (output_file.IsZombie()) {
    throw std::runtime_error("Could not create ROOT output file");
    }

  TTree tree{"MNXB11_Temperature", "MNXB11_Temperature"};
    Int_t year, month, day;
    Int_t hour, minute, second;
    Double_t temperature;
    Char_t grading;

    //Make branches for all the data
    // &gives address of variable
    tree.Branch("year", &year);
    tree.Branch("month", &month);
    tree.Branch("day", &day);
    tree.Branch("hour", &hour);
    tree.Branch("minute", &minute);
    tree.Branch("second", &second);
    tree.Branch("temperature", &temperature);
    tree.Branch("grading", &grading);

for (const auto& measurement : measurements) {
    year = measurement.get_year();
    month = measurement.get_month();
    day = measurement.get_day();
    hour = measurement.get_hour();
    minute = measurement.get_minute();
    second = measurement.get_second();
    temperature = measurement.get_temperature();
    grading = measurement.get_grading();

    tree.Fill();
  }
  tree.Write(); //write tree to root file
  output_file.Close(); //close file- no leakage
}