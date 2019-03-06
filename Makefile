OBJ=obj
IDIR=src/includes
SRCS:=$(wildcard src/includes/*.hpp)
SRCS:=$(wildcard src/*.cpp)
OBJECTS=$(SRCS:.cpp=.o)
CPP=g++
CFLAGS=-std=c++11 

all: cspim

test: cspim
	./cspim -i test/test1.c

.cpp.o:
	$(CPP) $(CFLAGS) -c -o $@ $<

cspim: $(OBJECTS)
	$(CPP) $(CFLAGS) -o $@ $(OBJECTS)

clean:
	rm $(OBJECTS) cspim *.o