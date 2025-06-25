CXX = g++
# define the dependencies in a separate file
DEP_FILES = main.cpp

# define the flags and libraries for C++
CXXFLAGS := -std=c++11 -Wall $(shell pkg-config --cflags opencv4)
LDFLAGS := $(shell pkg-config --libs opencv4)

# define the build target
TARGET = ConvolutionAndCorrelation

# build the target using the C++ compiler
# $@ : Name of Target file being generated 
# $< : Name of first prerequisite in a rule
$(TARGET): $(DEP_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# delete the object file and the executable on clean
clean:
	rm -f $(TARGET) *.o
