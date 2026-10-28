
#!/bin/bash

#############################################
#cleandata.sh 
#Authors: Charlotte Rösler
#Description:
#    The file takes SHMI file in and cleans it 
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
if [[ ! -f "$INPUT" ]]; then
    echo "Error: File not found: $INPUT"
    exit 1
fi


#Extract file name
DATAFILE=$(basename "$INPUT" .csv)


#Check if file is directory
if [[ "x$INPUT" != "x" ]]; then
   # T4.2 check if the file is a directory, it should not be!
   # exit with error in case.
   if [[ -d $INPUT ]]; then
      echo -e "This script requires a data file and not a directory, exiting..."     
      exit 1
   fi
fi



# Display the first 30 lines
#echo "First 30 lines of the file:"
#head -n 30 "$INPUT"

# Find the measurement header
#echo "Measurement header:"
#grep -n "^Datum;Tid" "$INPUT"

# Count the total number of lines
#echo "Total number of lines:"
#wc -l "$INPUT"

# Extract actual measurement rows

#use the Date; Time; Temperature; Quality of the data
# and out it into 4 columns, which are sepearted by spaces
#then save in file clean_{filename}.csv
awk -F';' '
BEGIN { data = 0 }
/^Datum;Tid/ { data = 1; next }
data && /^[0-9][0-9][0-9][0-9]-[0-9][0-9]-[0-9][0-9];[0-9][0-9]:[0-9][0-9]:[0-9][0-9];[-+]?[0-9]+([.][0-9]+)?;[GY]/ {
    print $1 " " $2 " " $3 " " $4
}
' "$INPUT" > clean_${DATAFILE}.csv

# clean file if it already exist
REPORT="report_${DATAFILE}.txt"
> "$REPORT"


#not really sure what is happening
echo "Writing filesizes summary to report_${DATAFILE}"
for csvfile in *.csv; do
    SIZE=$(stat -c %s "$csvfile")
    echo "   file $csvfile has size: $SIZE" >> report_${DATAFILE}
done


