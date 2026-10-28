#pragma once

#include <string>
#include <vector>
#include <TObject.h>



class Measurement: public TObject{
public:
    Measurement();
    Measurement(int year, int month, int day,
                int hour, int minute, int second,
                double temperature, char grading);
    ~Measurement() override =default;

    int get_year() const;
    int get_month() const;
    int get_day() const;
    int get_hour() const;
    int get_minute() const;
    int get_second() const;

    double get_temperature() const;
    char get_grading() const;

    bool is_valid() const;

    ClassDef(Measurement, 1);


private:
    int year_;
    int month_;
    int day_;
    int hour_;
    int minute_;
    int second_;

    double temperature_;
    char grading_;

};

std::vector<Measurement> read_measurements(
    const std::string& filename
);

void persist_measurements(
    const std::vector<Measurement>& measurements,
    const std::string& output_filename
);



