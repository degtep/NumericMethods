CXX           = g++
CXXFLAGS      = -Wall
DEL_FILE      = rm -f

OBJECTS       = main.o

TARGET        = task2

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

clean:	
	-$(DEL_FILE) $(OBJECTS) 

main.o: main.cpp


