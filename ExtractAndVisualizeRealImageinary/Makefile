CC = g++
CFLAGS = -Wall `pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`
TARGET = realAndImaginary

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) -o $(TARGET) main.cpp $(LIBS)

clean:
	rm -f $(TARGET)
