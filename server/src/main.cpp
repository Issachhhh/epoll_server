#include "../include/socker_server.h"
#include "../include/thread_pool.h"
int main(int argc, char** argv) {
    unsigned short port = 10000;
    if(SocketSever::epollFd <= 0) {
        std::cout << "create epoll failed." << SocketSever::epollFd << std::endl;
        return 1;
    }
    SocketSever sever(port);
    epoll_event events[MAX_EVENT];
    ThreadPool tp(10);
    int checkpos = 0;
    while(1) {
        long now = time(NULL);
        for(int i = 0; i < 100; ++i, ++checkpos) {
            if(checkpos == MAX_EVENT) checkpos = 0;
            if(sever.g_event[checkpos].getStatus() != 1) continue;
            long duration = now - sever.g_event[checkpos].last_active;
            if(duration > 60) {
                close(sever.g_event[checkpos].fd);
                std::cout << "[fd=" << sever.g_event[checkpos].fd << "] timeout";
                sever.g_event[checkpos].event_del(SocketSever::epollFd);
            }
        }
        int fds = epoll_wait(SocketSever::epollFd, events, MAX_EVENT, 100000);
        for(int i = 0; i < fds; ++i) {
            MyEvent* ev = static_cast<MyEvent*>(events[i].data.ptr);
            tp.addTask(ev);
            // std::cout << fds << std::endl;
            // if((events[i].events&EPOLLIN)&&(ev->events&EPOLLIN)) // read event  
            // {  
            //     ev->call_back(ev->fd, events[i].events, ev, ev->sock);  
            // }  
            // if((events[i].events&EPOLLOUT)&&(ev->events&EPOLLOUT)) // write event  
            // {  
            //     ev->call_back(ev->fd, events[i].events, ev, ev->sock);  
            // }
            std::cout << "over" << std::endl;
        }
    }
}