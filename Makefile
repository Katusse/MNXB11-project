CXX := g++
CXXWARNINGS := -Wall -Wextra -Werror
CXXOPT := -O3
CXXSTD := -std=c++17
INCLUDES := -I include
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs)
CXXFLAGS := -fPIC $(shell root-config --cflags) $(CXXWARNINGS) $(CXXSTD) $(CXXOPT) $(INCLUDES) $(ROOTCFLAGS) 
LDFLAGS := $(ROOTLIBS)
BUILD := build
LIB   := $(BUILD)/libMNXB11.so

.PHONY: all clean

all: libMNXB11.so main

# Generate the ROOT dictionary for Measurement.
Dict.cxx: include/load_data.h LinkDef.h
	rootcling -f $@ -Iinclude include/load_data.h LinkDef.h

# Compile the ROOT dictionary.
Dict.o: Dict.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files in src/.
src/%.o: src/%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build the shared library.
libMNXB11.so: src/load_data.o src/dataset_registry.o Dict.o
	$(CXX) -shared -o $@ $^ $(ROOTLIBS)

# Compile main.cxx.
main.o: main.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link the executable against the shared library and ROOT.
main: main.o src/run_Love_analysis.o src/extremedays.o src/run_weather.o src/yearly_average.o libMNXB11.so
	$(CXX) -o $@ main.o src/run_Love_analysis.o src/extremedays.o src/run_weather.o src/yearly_average.o -L. -lMNXB11 $(LDFLAGS) -Wl,-rpath,'$$ORIGIN'

# Remove generated files.
clean:
	rm -fv src/*.o main.o Dict.cxx Dict.o libMNXB11.so main