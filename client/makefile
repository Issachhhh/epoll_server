all: compile

compile: main.o
	g++ -o client main.o

test.o:main.cpp
	g++ -c main.cpp -std=c++11 -g

clean:
	rm *.o
