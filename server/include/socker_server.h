#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include "../include/my_event.h"
#include "../include/thread_pool.h"
constexpr int MAX_EVENT = 500;
constexpr int MAX_thread = 100;
class SocketSever{
public:
    SocketSever(short port);
    ~SocketSever();
    MyEvent g_event[MAX_EVENT+1];
    static int epollFd;
private:
    int g_epollFd;
};

void sendData(int fd, int events, void* arg, void* sock);
void recvData(int fd, int events, void* arg, void* sock);
void acceptConn(int fd, int events, void* arg, void* sock);