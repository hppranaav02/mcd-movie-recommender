# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Source files and output executable
SRC_FILES = movie_rate.cpp
EXECUTABLE = MovieRate

# Object files (automatically generated from SRC_FILES)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Targets and rules
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)

.PHONY: all clean
