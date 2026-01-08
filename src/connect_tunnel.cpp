#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>

void tunnel(SOCKET from, SOCKET to) {
    char buffer[4096];
    int bytes;
    while ((bytes = recv(from, buffer, sizeof(buffer), 0)) > 0) {
        send(to, buffer, bytes, 0);
    }
    shutdown(to, SD_SEND);
}

void handleConnect(SOCKET clientSocket, const std::string& host, int port) {
    addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(host.c_str(), std::to_string(port).c_str(), &hints, &res);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    connect(serverSocket, res->ai_addr, res->ai_addrlen);
    freeaddrinfo(res);

    const char* ok =
        "HTTP/1.1 200 Connection Established\r\n\r\n";
    send(clientSocket, ok, strlen(ok), 0);

    std::thread t1(tunnel, clientSocket, serverSocket);
    std::thread t2(tunnel, serverSocket, clientSocket);

    t1.join();
    t2.join();

    closesocket(serverSocket);
    closesocket(clientSocket);
}
