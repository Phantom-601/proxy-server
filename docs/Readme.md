# HTTP/HTTPS Proxy Server

This project is a **custom forward proxy server** implemented in **C++ using Winsock** on Windows.  
It forwards HTTP traffic, supports HTTPS tunneling, applies domain-based access control, and logs each request.

---

## Features

-   HTTP request forwarding
-   HTTPS support via CONNECT tunneling
-   Domain-based blocking using a configuration file
-   Concurrent client handling
-   Timestamped request logging

---

## Requirements

-   Windows 10 / 11 (64-bit)
-   MinGW-w64 (POSIX, SEH) with GCC ≥ 10
-   C++17 compatible compiler
-   `curl` (for basic verification)

---

## Project Structure

proxy-project/  
├── src/ # Source files  
├── include/ # Header files  
├── config/ # Blocklist configuration  
├── logs/ # Proxy logs  
├── docs/ # Design and readme  
└── tests/ #testing file

---

## Build Instructions

From the project root directory:

```bat
g++ -std=c++17 -pthread src/*.cpp -Iinclude -lws2_32 -o proxy.exe
proxy.exe
```

The proxy listens on port 8888 by default.

## Configuration

### Blocked Domains

Edit this file:

```bat
config/blocked_domains.txt
```

Add one domain per line (case-insensitive):

```bat
super.com
amazon.com
```

Restart the proxy after modifying the blocklist.

## Logs

Runtime Logs are written to:

```bash
    logs/proxy.log
```

Each log entry contains:

-   Timestamp
-   Destination host
-   HTTP request line
-   Action (ALLOWED or BLOCKED)

Example:

```bash
2026-01-04 15:42:10 | example.com | GET http://example.com/ HTTP/1.1 | ALLOWED
```

## Demo Video

Here is the Domonstration of complete proxy server features.

[click here](https://iitracin-my.sharepoint.com/:v:/g/personal/pankaj_k1_ece_iitr_ac_in/IQCP70ewZ7bkRrQfTntrqOZ6AcFZDUtfdYQjCBcgFrie0e4?e=3tQe0L)
