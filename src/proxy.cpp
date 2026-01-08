#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <string>

#include "proxy.h"

#pragma comment(lib, "ws2_32.lib")

void handleConnect(SOCKET clientSocket, const std::string& host, int port);

std::string currentTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    localtime_s(&tm, &t);   

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

void writeLog(const std::string& entry) {
    static std::mutex logMutex;
    std::lock_guard<std::mutex> lock(logMutex);

    std::ofstream log("logs/proxy.log", std::ios::app);
    log << entry << std::endl;  
}

void handleClient(SOCKET clientSocket) {
    char buffer[4096];
    std::string request;

    while (request.find("\r\n\r\n") == std::string::npos) {
        int bytes = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytes <= 0) {
            closesocket(clientSocket);
            return;
        }
        request.append(buffer, bytes);
    }

    std::istringstream reqStream(request);
    std::string requestLine;
    std::getline(reqStream, requestLine);

    std::string method, target, version;
    std::istringstream rl(requestLine);
    rl >> method >> target >> version;

    std::string host;
    std::string line;
    while (std::getline(reqStream, line)) {
        if (line.rfind("Host:", 0) == 0) {
            host = line.substr(5);
            break;
        }
    }

    host.erase(0, host.find_first_not_of(" \t\r\n"));
    host.erase(host.find_last_not_of(" \t\r\n") + 1);
    std::transform(host.begin(), host.end(), host.begin(), ::tolower);

    std::string logPrefix =
        currentTimestamp() + " | " +
        host + " | " +
        requestLine + " | ";

    if (blockedDomains.count(host)) {
        const char* forbidden =
            "HTTP/1.1 403 Forbidden\r\n"
            "Content-Length: 24\r\n\r\n"
            "Access denied by proxy";

        send(clientSocket, forbidden, strlen(forbidden), 0);
        writeLog(logPrefix + "BLOCKED");
        closesocket(clientSocket);
        return;
    }

    writeLog(logPrefix + "ALLOWED");

    if (method == "CONNECT") {
        size_t colonPos = target.find(':');
        if (colonPos == std::string::npos) {
            closesocket(clientSocket);
            return;
        }

        std::string connectHost = target.substr(0, colonPos);
        int port = std::stoi(target.substr(colonPos + 1));

        handleConnect(clientSocket, connectHost, port);
        return;
    }

    addrinfo hints{}, *res = nullptr;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), "80", &hints, &res) != 0) {
        closesocket(clientSocket);
        return;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connect(serverSocket, res->ai_addr, res->ai_addrlen) != 0) {
        freeaddrinfo(res);
        closesocket(serverSocket);
        closesocket(clientSocket);
        return;
    }

    freeaddrinfo(res);

    send(serverSocket, request.c_str(), (int)request.size(), 0);

    int bytes;
    while ((bytes = recv(serverSocket, buffer, sizeof(buffer), 0)) > 0) {
        send(clientSocket, buffer, bytes, 0);
    }

    closesocket(serverSocket);
    closesocket(clientSocket);
}
