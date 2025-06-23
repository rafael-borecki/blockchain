CXX        := g++
CXXFLAGS   := -std=c++20 -Wall -Wextra
LDFLAGS    := -lcrypto $(shell pkg-config --libs rdkafka++) -pthread

SOURCES    := Time.cpp Block.cpp Worker.cpp HashUtils.cpp DataProducer.cpp Conductor.cpp SaveData.cpp main.cpp KafkaHelpers.cpp KafkaThreads.cpp
OBJECTS    := $(SOURCES:.cpp=.o)
EXECUTABLE := blockchain
DATA		   := *.bin

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean run
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(DATA)
