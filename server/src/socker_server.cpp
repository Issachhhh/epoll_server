#include "../include/socker_server.h"
#include <cstring>
int SocketSever::epollFd = epoll_create(MAX_EVENT);
SocketSever::SocketSever(short port){
    for(int i = 0; i < MAX_EVENT; ++i)
        this->g_event[i] = MyEvent();
    int listenFd = socket(AF_INET, SOCK_STREAM, 0);
    // std::cout << listenFd << std::endl;
    int flags = fcntl(listenFd, F_GETFL, 0);
    if(fcntl(listenFd, F_SETFL, O_NONBLOCK |flags) < 0) {
        std::cout << __func__ << ": fcntl nonblocking set failed" << std::endl;
    } else {
        std::cout << __func__ << ": fcntl nonblocking set success" << std::endl;
    }
    this->g_event[MAX_EVENT].MyEventSet(listenFd, acceptConn,
                                        &(this->g_event[MAX_EVENT]), this);
    this->g_event[MAX_EVENT].event_add(epollFd, EPOLLIN | EPOLLET);
    sockaddr_in sin;
    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    const char* localip = "192.168.159.129";
    in_addr addr;
    if (inet_pton(AF_INET, localip, &addr) != 1) {
        perror("inet_pton error");
        exit(1);
    }
    sin.sin_addr = addr;
    sin.sin_port = htons(port);
    bind(listenFd, (const sockaddr*)&sin, sizeof(sin));
    listen(listenFd, 5);
}

SocketSever::~SocketSever() {
}

void sendData(int fd, int events, void *arg, void *sock) {
    std::cout << "in sendData" << std::endl;
    SocketSever* s = static_cast<SocketSever*>(sock);
    MyEvent* ev = static_cast<MyEvent*>(arg);
    std::cout << __func__ << (void*)(ev->buff) << std::endl;
    strcpy(ev->buff, "hello");
    ev->buff[5] = '\0';
    int len = send(fd, ev->buff, 6, 0);
    if(len > 0) {
        std::cout << "send[fd=" << fd << "], [" << len << "<-> "
                  << ev->len << "]" << ev->buff << std::endl;
        ev->s_offset += len;
        ev->event_del(SocketSever::epollFd);
        ev->MyEventSet(fd, recvData, arg, sock);
        ev->event_add(SocketSever::epollFd, EPOLLIN | EPOLLET);
    } else {
        ev->event_del(SocketSever::epollFd);
        std::cout << "send[fd=" << fd << "] error[" << errno <<"]" << std::endl;
        close(ev->fd);
    }
}

void recvData(int fd, int events, void *arg, void *sock)
{
    std::cout << "in recvData" << std::endl;
    SocketSever* s = static_cast<SocketSever*>(sock);
    MyEvent* ev = static_cast<MyEvent*>(arg);
    int len = recv(fd, ev->buff, sizeof(ev->buff), 0);
    ev->event_del(SocketSever::epollFd);
    if(len > 0) {
        ev->len += len;
        ev->buff[len] = '\0';
        std::cout << "C[" << fd  << (void*)ev->buff<< "]:" << ev->buff << std::endl;
        ev->MyEventSet(fd, sendData, arg, sock);
        ev->event_add(SocketSever::epollFd, EPOLLOUT | EPOLLET);
        // ev->MyEventSet(fd, recvData, arg, sock);
        // ev->event_add(SocketSever::epollFd, EPOLLIN | EPOLLET);
    } else if(len == 0){
        std::cout << __func__ << ":[fd=" << ev->fd << "] pos[" << ev-s->g_event << "]"
                  << std::endl;
        if(close(fd) == 0)
            std::cout << __func__ << ": close success" << std::endl;
    } else {
        ev->MyEventSet(fd, recvData, arg, sock);
        ev->event_add(SocketSever::epollFd, EPOLLIN | EPOLLET);
        std::cout << "recv[fd=" << fd << "] error[" << errno << "]:"
                  << strerror(errno) << std::endl;
    }
}

void acceptConn(int fd, int events, void *arg, void *sock) {
    std::cout << "in acceptConn" << std::endl;
    sockaddr_in sin;
    socklen_t len = sizeof(sockaddr_in);
    int nfd, i;
    SocketSever* s = static_cast<SocketSever*>(sock);
    if((nfd = accept(fd, (sockaddr*)&sin, &len)) == -1) {
        if(errno != EAGAIN && errno != EINTR) {}
        std::cout << __func__ << ": accept, " << errno;
    }
    do{
        for(i = 0; i < MAX_EVENT; ++i) {
            std::cout << "i = " << i << ", &(s->g_event[i]) = " << &(s->g_event[i]) << std::endl;
            if((s->g_event)[i].getStatus() == 0) {
                break;
            }
        }
        std::cout << "in acceptConn" << std::endl;
        if(i == MAX_EVENT) {
            std::cout << __func__ << "max connextion limit[" << MAX_EVENT <<"]." << std::endl;
            break;
        }
        int flags = fcntl(nfd, F_GETFL, 0);
        int iret = 0;
        if((iret = fcntl(nfd, F_SETFL, O_NONBLOCK | flags)) < 0) {
            std::cout << __func__ << ": fcntl nonblocking set failed: " << errno << std::endl;
            break;;
        } else {
            std::cout << __func__ << ": fcntl nonblocking set success" << std::endl;
        }
        s->g_event[i].MyEventSet(nfd, recvData, &(s->g_event[i]), sock);
        s->g_event[i].event_add(SocketSever::epollFd, EPOLLIN | EPOLLET);
    }while(0);
    std::cout << "new conn[" << inet_ntoa(sin.sin_addr) << ":" << ntohs(sin.sin_port)
              << "]" << "[time: " << s->g_event[i].last_active << "], pos[" << i << "]."
              << std::endl; 
}