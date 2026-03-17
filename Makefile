CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = sha_program

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(OBJ) $(TARGET)
