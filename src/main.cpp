#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>      
#include <iostream>
#include <fstream>
#include <set>
#include <algorithm>
#include "proxy.h"


#pragma comment(lib, "ws2_32.lib")

std::set<std::string> blockedDomains;

void loadBlocklist() {
    std::ifstream file("config/blocked_domains.txt");
    std::string domain;
    while (std::getline(file, domain)) {
        domain.erase(0, domain.find_first_not_of(" \t\r\n"));
        domain.erase(domain.find_last_not_of(" \t\r\n") + 1);
        std::transform(domain.begin(), domain.end(), domain.begin(), ::tolower);
        if (!domain.empty())
            blockedDomains.insert(domain);
    }
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    loadBlocklist();

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(listenSocket, (sockaddr*)&addr, sizeof(addr));
    listen(listenSocket, SOMAXCONN);

    std::cout << "Proxy listening on port 8888\n";

    while (true) {
        SOCKET client = accept(listenSocket, nullptr, nullptr);
        std::thread(handleClient, client).detach();
    }

    WSACleanup();
}
