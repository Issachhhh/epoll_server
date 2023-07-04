#include "../include/my_event.h"

void MyEvent::MyEventSet(int _fd, void (*_call_back)(int, int, void*, void*), void* _args, void* sock){
    this->fd = _fd;
    this->call_back = _call_back;
    this->events = 0;
    this->status = 0;
    this->len = 0;
    this->last_active = time(NULL);
    bzero(this->buff, sizeof(this->buff));
    this->args = _args;
    this->sock = sock;
}

void MyEvent::event_add(int epoll_fd, int events) {
    epoll_event epv = {0, {0}};
    int op;
    epv.data.ptr = this;
    epv.events = this->events = events;
    std::cout << "status:" << this->status << std::endl;
    if(this->status == 1) {
        op = EPOLL_CTL_MOD;
    } else {
        op = EPOLL_CTL_ADD;
        this->status = 1;
    }
    if(epoll_ctl(epoll_fd, op, this->fd, &epv) < 0) {
        std::cout << "Event Add failed[fd=" << this->fd
                  << "], evnets["<< events << "]" << errno << std::endl;
    } else {
        std::cout << "Event Add OK[fd=" << this->fd << "], op=" << op
                  << ", evnets[" << events <<"]" << std::endl;
    }
}

void MyEvent::event_del(int epoll_fd) {
    if(this->status == 0) return;
    epoll_event epv = {0, {0}};
    this->status = 0;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_DEL, this->fd, &epv) < 0) {
        std::cout << "Event del false, fd = " << this->fd << std::endl;
    } else {
        std::cout << "Event del success, fd = " << this->fd << std::endl;
    }
}

int MyEvent::getStatus() {
    return this->status;
}

void MyEvent::setStatus(int _status) {
    this->status = _status;
}