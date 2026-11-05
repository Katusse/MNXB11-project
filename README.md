# MNXB11-project


# Directory structure

The base directory contains main.cxx which runs the code from src.
Additionally the base directory contains this README, a changelog with our changes and a Dict.cxx. 

The folder `src/`contains the source code nedded to run the analysis for each task and needed to create a root Tree with the data.
The folder `datasets/` includes the datasets, a bash programme to clean the datasets and the cleaned dtatasets.
The directory  `include/` contains all header files.



The main files that can be a starting point are:


- [main.cxx](main.cxx)
  - This file contains a `main` function for your project, which runs the project. The name of the dataset can be passed into main
- [Makefile](Makefile)
  - This file gives the `make` utility instructions on how to build different targets. For your convenience there is also a `clean` target that can be used to delete built files to start anew.
  - You should use this file if you plan to automate the C++ build of your project, but don't use it as a script. It is meant for C++ stuff only - use a BASH script for environment variables and task automation instead.

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
*`run_weather.cxx`contains the prgramme that loads the data with the help from function from load_data.cxx and makes a Tree called in weather_data.root that is called MNXB11_temperature. The tree stores all relevant info like all dates and times, the temperature measurement and the grading of the measurement. It can be run by passing in citynames
*`load_data.cxx`defines the functions to load the data
*`yearly_average.cxx` runs the analysis for yearly average temperatures and returns a histogram contianing the data. It accesses the weather_data.root

The [scripts](scripts) folder contains BASH scripts examples that can be used to create a build script or a launcher/wrapper for your binaries or for your SLURM submission

The [slurm](slurm) folder contains an example SLURM SBATCH script that calls one of the project launcher scripts above.


# Building the project

We have included a basic Makefile here which should be familiar to you. It follows the same project structure that we have been using in the course. When you add a new translation unit to the project, you have update the dependencies in the Makefile. 

By default, the `all` target will be run which 
- Compiles any `.cxx` files in the `src/` directory into object files 
- Compiles [`main.cxx`](main.cxx) and links with all the object files in `src/`

You can run the `clean` target (`make clean`) to remove any object files that have been produced as well as the `main` executable.


