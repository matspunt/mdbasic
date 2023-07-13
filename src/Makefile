CXX = g++
CXXFLAGS = -Wall -Wextra -O2
LDFLAGS =

SRCS = atom.cpp main.cpp system.cpp
OBJS = $(SRCS:.cpp=.o)
DEPS = atom.hpp system.hpp

TARGET = md

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
