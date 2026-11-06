# MNXB11-project


# Directory structure

The base directory contains main.cxx which runs the code from src.
Additionally the base directory contains this README and a changelog with our changes. 

The folder `src/`contains the source code nedded to run the analysis for each task and needed to create a root Tree with the data.
The folder `datasets/` includes the datasets, a bash programme to clean the datasets and the cleaned dtatasets.
The directory  `include/` contains all header files.



The main files:


- [main.cxx](main.cxx)
  - This file contains a `main` function for the project.
  - It takes two arguments: The function itself and a city name.
  - If no city name is given it defaults to Lund.
  - It then runs all analyses on that citys data.
  - And then erases our created tree.
- [Makefile](Makefile)
  - This file gives the `make` utility instructions on how to build different targets. It also has a `clean` target that can be used to delete built files to start anew.
  

We have also included three special files in the base of the repository 
- [.gitignore](.gitignore)
  - This file contains regular expressions that git tells git that it shouldn't add certain file to your repository. 
  - Your git repository should generally not contain binary files like object files or executables nor should it contain build artefacts like external libraries. 
- [.clang-format](.clang-format)
  - This file holds the configuration for the clang-format tool that you can use to format your code consistently 
  - It is a good idea to keep your code formatted in a consistent manner, especially when working in groups but doing it manually is a waste of your time. Use a tool for it!
  ```
  # Show what the src/Example.cxx file would look like if formatted
  clang-format src/Example.cxx 
  # Carry out the formatting in the file directly 
  clang-format src/Example.cxx -i
  ```
  - The `.clang-format` file holds the configuration that clang-format will use to determine how to format your code. By default, it will be formatted according to Google's style but you can pick any that you like from https://clang.llvm.org/docs/ClangFormatStyleOptions.html

The [datasets](datasets) folder contains open data from SMHI and a README.md with further information about it.
It further contains a bash prgramme `cleandata.sh`that takes a dataset and makes a clean_dataset.csv file out of it.
The cleaned datasets are stored in `datasets/`, too.

The [src](src) folder contains all programmes.
*`dataset_registry.cxx`shortens all of our datasets into string names that the files can use to call on them.
*`extremedays.cxx`runs the analysis for how many days were hottest or coldest in a year, it uses functions from load_data.cxx directly rather than accessing weather_data.root 
*`run_Love_analysis`is a programme that gets a dataset and then runs the functions from extremedays.cxx on it
*`run_weather.cxx`contains the prgramme that loads the data with the help from function from load_data.cxx and makes a Tree called in weather_data.root that is called MNXB11_temperature. The tree stores all relevant info like all dates and times, the temperature measurement and the grading of the measurement. It can be run by passing in citynames
*`load_data.cxx`defines the functions to load the data
*`yearly_average.cxx` runs the analysis for yearly average temperatures and returns a histogram containing the data. It accesses the weather_data.root
*`daily_average.cxx` takes the weather_data.root and makes the Mean temperature on each day of the year histogram.

The [scripts](scripts) folder contains BASH scripts examples that can be used to create a build script or a launcher/wrapper for your binaries or for your SLURM submission

The [slurm](slurm) folder contains an example SLURM SBATCH script that calls one of the project launcher scripts above.


# Building the project

  Enter a rootapptainer and use the command `make`.
  - The Makefile will compile all of our different src and header files as well as making a library out of our data and a generated C++ library.
  - It then links all of our different analyses and our library through main.

# Running the project

  Use ./main to run the main file in the current directory.
  As a default it uses Lund for the analyses, type any other cities name after ./main in order to change this (for cities with å in them, use a instead).



