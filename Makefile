all: cspim

cspim: main.o
	g++ main.o -o cspim

main.o: src/main.cpp
	g++ -std=c++11 -c src/main.cpp -o main.o