#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

using namespace std;

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cout << "Failed to create socket." << endl;
        return 1;
    }
    
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(10000);
    inet_pton(AF_INET, "192.168.159.129", &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        cout << "Failed to connect to the server." << endl;
        return 1;
    }
    int flag = fcntl(sock, F_GETFL, 0);
    const char* message = "hello";
    if (send(sock, message, strlen(message), 0) == -1) {
        cout << "Failed to send message to the server." << endl;
        return 1;
    }
    char recvbuff[20];
    if (recv(sock, recvbuff, strlen(message), 0) == -1) {
        cout << "Failed to recv message to the server." << endl;
        close(sock);
        return 1;
    } else {
        recvbuff[strlen(message)] = '\0';
        cout << "recv data: " << recvbuff << endl;
    }
    shutdown(sock, SHUT_RDWR);

    return 0;
}
