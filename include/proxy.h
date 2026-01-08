#pragma once
#include <winsock2.h>
#include <string>
#include <set>

void handleClient(SOCKET clientSocket);
extern std::set<std::string> blockedDomains;
