CXX := g++
CXXWARNINGS := -Wall -Wextra -Werror
CXXOPT := -O3
CXXSTD := -std=c++17
INCLUDES := -I include

ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs)

CXXFLAGS := -fPIC $(CXXWARNINGS) $(CXXSTD) $(CXXOPT) $(INCLUDES) $(ROOTCFLAGS)

LIB := build/libMNXB11.so
EXE := build/main

.PHONY: all clean

all: $(EXE)

# Create build directory
build:
	mkdir -p build

# Generate the ROOT dictionary
build/Dict.cxx: include/load_data.h include/LinkDef.h | build
	rootcling -f $@ -Iinclude include/load_data.h include/LinkDef.h

# Compile the ROOT dictionary
build/Dict.o: build/Dict.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files in src/
build/%.o: src/%.cxx | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build the shared library
$(LIB): build/load_data.o build/dataset_registry.o build/Dict.o
	$(CXX) -shared -o $@ $^ $(ROOTLIBS)

# Compile main.cxx
build/main.o: main.cxx | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link executable
$(EXE): build/main.o \
        build/run_Love_analysis.o \
        build/extremedays.o \
        build/run_weather.o \
        build/yearly_average.o \
        build/yearly_extremes.o \
        build/daily_average.o \
        $(LIB)
	$(CXX) -o $@ $(filter %.o,$^) -Lbuild -lMNXB11 $(ROOTLIBS) \
	    -Wl,-rpath,'$$ORIGIN'

# Remove generated files
clean:
	rm -rf build
