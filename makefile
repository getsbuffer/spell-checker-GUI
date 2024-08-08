CC = g++
CFLAGS = -Wall -std=c++11 `wx-config --cxxflags`
LDFLAGS = `wx-config --libs`

SOURCES = myspellGUI.cpp
HEADERS = hashtable.h hashtable.hpp myspellGUI.h
OBJECTS = $(SOURCES:.cpp=.o)

TARGET = SpellChecker

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LDFLAGS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
