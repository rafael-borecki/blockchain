CXX        := g++
CXXFLAGS   := -std=c++20 -Wall -Wextra
LDFLAGS    := -lcrypto

SOURCES    := Time.cpp Block.cpp Worker.cpp HashUtils.cpp main.cpp
OBJECTS    := $(SOURCES:.cpp=.o)
EXECUTABLE := main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	$(./EXECUTABLE)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

.PHONY: all clean

