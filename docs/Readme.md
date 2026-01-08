# Custom HTTP/HTTPS Proxy Server

## Overview

This project implements a **forward proxy server** in **C++ using Winsock** on Windows.  
The proxy acts as an intermediary between clients and destination servers, forwarding HTTP traffic and tunneling HTTPS connections while enforcing domain-based access control.

The implementation focuses on **core networking and systems concepts** such as TCP sockets, HTTP parsing, concurrency, and thread-safe logging.

---

## Key Features

-   HTTP request forwarding over TCP
-   HTTPS tunneling using the CONNECT method
-   Domain-based blocking using a configurable blocklist
-   Concurrent client handling (thread-per-connection)
-   Real-time, thread-safe logging with timestamps
-   Modular and extensible code structure

---

## Architecture

The proxy plays a dual role:

-   **Server**: accepts incoming client connections
-   **Client**: establishes outbound connections to destination servers
