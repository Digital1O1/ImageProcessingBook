CXX = g++
# define the dependencies in a separate file
#OBJECTS = main.cpp

# define the flags and libraries for C++
CXXFLAGS := -std=c++11 -Wall $(shell pkg-config --cflags opencv4)
LDFLAGS := $(shell pkg-config --libs opencv4)


# define the build target
TARGET1 = ConvolutionAndCorrelation
TARGET2 = BoarderExample

# Creatse the 'all' target
# Default target that builds the entire project that's dependent on the executable 
all: $(TARGET1) $(TARGET2)


# build the target using the C++ compiler
# $@ : Name of Target file being generated 
# $< : Name of first prerequisite in a rule
$(TARGET1): main.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

$(TARGET2): copyMakeBoarderExample.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# delete the object file and the executable on clean
clean:
	rm -f $(TARGET1) $(TARGET2) *.o