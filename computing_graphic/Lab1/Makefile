# Compiler settings
CXX      = g++
CXXFLAGS = -Wall -std=c++17 `wx-config --cxxflags --libs`

# Source files
SRCS = src/main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = main_app

# Include directories
INCLUDES = -I.

# Linker flags
LDFLAGS = `wx-config --libs`

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean