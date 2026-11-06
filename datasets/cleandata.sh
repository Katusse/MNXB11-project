#!/bin/bash

#############################################
#cleandata.sh 
#Authors: Charlotte Rösler
#Description:
#    The file takes SMHI file in and cleans it 
#    so that it can read by c++
#    The produced output is in the format of 
#    <YYYY>-<MM>-<DD> <HH>:<MM>:<SS> <Temperature> <Y/G>
#    <G> means data that is controlled
#    <Y> mean data that is not controlled or qualified
#    An output file with the name clean_<INPUT>.csv
#    and a report_<INPUT>.txt with information
#Run:
#    By bash cleandata.sh <to_cleaned_file.csv>
#
#Uses the code example from https://github.com/floridop/MNXB11-BASH/blob/main/casestudy/code/smhicleaner.sh.solution
#
#############################################################


#Provides info on how to run the file, no input, but error output with info

usage(){
	echo "----"
	echo -e "  To call this script please use"
	echo -e "   $0 '<path-to-datafile>'"
	echo -e "  Example:"
    echo -e "   $0 '/tutorial3/casestudy/data/smhi-opendata_1_52240_20200905_163726.csv'"
	echo "----"
}

# Name of the input file, takes in the first bah argument
INPUT="$1"

# Check if the file exists
#if [[the input path does not exist or is empty]]
if [[ ! -f "$INPUT" ]]; then
    echo "Error: Input must be an existing regular file: $INPUT"
    usage
    exit 1
fi


#Extract file name
#basename removes the path and the .csv extension from the input file name
DATAFILE=$(basename "$INPUT" .csv)


# Extract actual measurement rows
#specify separator a semicolon, and use awk to filter the data
#use the Date; Time; Temperature; Quality of the data
#start reading the data after the header line, and only read lines with below format
# and out it into 4 columns, which are sepearted by spaces
#then save in file clean_{filename}.csv
awk -F';' ' 
BEGIN { data = 0 }
/^Datum;Tid/ { data = 1; next }
data && /^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9];[0-9][0-9]:[0-9][0-9]:[0-9][0-9];[-+]?[0-9]+([.][0-9]+)?;[GY]/ {
    print $1 " " $2 " " $3 " " $4
}
' "$INPUT" > "clean_${DATAFILE}.csv"
# output file is overwritten if it exists before

#write a report file with the name report_<filename>.txt
REPORT="report_${DATAFILE}.txt"
> "$REPORT"

echo "Writing filesizes summary to $REPORT"

#check the size of the cleaned file and the original file, and write it to the REPORT
csvfile="clean_${DATAFILE}.csv"
SIZE=$(stat -c %s "$csvfile") #retrieve the size of the file in bytes
echo "   file $csvfile has size: $SIZE" >> "$REPORT"

csvfile="$INPUT"
SIZE=$(stat -c %s "$csvfile") #retrieve the size of the file in bytes
echo "   file $csvfile has size: $SIZE" >> "$REPORT"


