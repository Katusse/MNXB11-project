CXX := g++
CXXWARNINGS := -Wall -Wextra -Werror
CXXOPT := -O3
CXXSTD := -std=c++17
INCLUDES := -I include

ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs)

CXXFLAGS := -fPIC $(CXXWARNINGS) $(CXXSTD) $(CXXOPT) $(INCLUDES) $(ROOTCFLAGS)
LDFLAGS := $(ROOTLIBS)

BUILD := build
LIB := $(BUILD)/libMNXB11.so
EXE := $(BUILD)/main

.PHONY: all clean

all: $(EXE)

# Create build directory
$(BUILD):
	mkdir -p $(BUILD)

# Generate the ROOT dictionary
$(BUILD)/Dict.cxx: include/load_data.h include/LinkDef.h | $(BUILD)
	rootcling -f $@ -Iinclude include/load_data.h include/LinkDef.h

# Compile the ROOT dictionary
$(BUILD)/Dict.o: $(BUILD)/Dict.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile source files in src/
$(BUILD)/%.o: src/%.cxx | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build the shared library
$(LIB): $(BUILD)/load_data.o $(BUILD)/dataset_registry.o $(BUILD)/Dict.o
	$(CXX) -shared -o $@ $^ $(LDFLAGS)

# Compile main.cxx
$(BUILD)/main.o: main.cxx | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile analysis source files
$(BUILD)/run_Love_analysis.o: src/run_Love_analysis.cxx | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/extremedays.o: src/extremedays.cxx | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/run_weather.o: src/run_weather.cxx | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/yearly_average.o: src/yearly_average.cxx | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/yearly_extremes.o: src/yearly_extremes.cxx | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/daily_average.o: src/daily_average.cxx | $(BUILD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link executable
$(EXE): $(BUILD)/main.o \
        $(BUILD)/run_Love_analysis.o \
        $(BUILD)/extremedays.o \
        $(BUILD)/run_weather.o \
        $(BUILD)/yearly_average.o \
        $(BUILD)/yearly_extremes.o \
        $(BUILD)/daily_average.o \
        $(LIB)
	$(CXX) -o $@ $(filter %.o,$^) -L$(BUILD) -lMNXB11 $(LDFLAGS) \
	    -Wl,-rpath,'$$ORIGIN'

# Remove generated files
clean:
	rm -rf $(BUILD)
