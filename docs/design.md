# Design – HTTP/HTTPS Proxy Server

## Overview

This project implements a **forward proxy server** in **C++ using Winsock** on Windows.  
The proxy forwards HTTP traffic and tunnels HTTPS connections while enforcing domain-based access control.

---

## Architecture

The proxy acts as both:

-   **Server**: accepts incoming client connections
-   **Client**: connects to destination servers

Client → Proxy → Destination Server
