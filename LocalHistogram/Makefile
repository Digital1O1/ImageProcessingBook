# Define the C++ compiler
CXX = g++

# Compiler flags: set C++11 standard and use pkg-config to get OpenCV compile flags.
CXXFLAGS = -std=c++11 `pkg-config --cflags opencv4`

# Linker flags: use pkg-config to get OpenCV libraries.
LDFLAGS = `pkg-config --libs opencv4`

# Name of the executable
TARGET = localHistogram

# Default target: build the executable
all: $(TARGET)

# Link the object file(s) to create the executable
$(TARGET): main.o
	$(CXX) -o $(TARGET) main.o $(LDFLAGS)

# Compile the source file into an object file
main.o: main.cpp
	$(CXX) -c main.cpp $(CXXFLAGS)

# Clean target to remove build files
clean:
	rm -f $(TARGET) main.o
