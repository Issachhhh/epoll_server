#ifndef MY_EVENT_H_
#define MY_EVENT_H_
#include <string.h>
#include <time.h>
#include <sys/epoll.h>
#include <iostream>
class MyEvent{
public:
    void MyEventSet(int _fd, void (*_call_back)(int, int, void*, void*), void* _arg, void* sock);
    void event_add(int epoll_fd, int events);
    void event_del(int epoll_fd);
    int getStatus();
    void setStatus(int _status);
    long last_active;
    char buff[128];
    int len, s_offset;
    int fd;
    int events;
    void (*call_back)(int fd, int events, void* args, void* sock);
    void* args;
    void* sock;
    int status;
};
#endif