all: compile

compile: main.o my_event.o socker_server.o thread_pool.o
	g++ -o epoll_server main.o my_event.o socker_server.o thread_pool.o

main.o:src/main.cpp
	g++ -c src/main.cpp -std=c++11 -g

my_event.o:src/my_event.cpp
	g++ -c src/my_event.cpp -std=c++11 -g

socker_server.o:src/socker_server.cpp
	g++ -c src/socker_server.cpp -std=c++11 -g

thread_pool.o:src/thread_pool.cpp
	g++ -c src/thread_pool.cpp -std=c++11 -g

clean:
	rm *.o