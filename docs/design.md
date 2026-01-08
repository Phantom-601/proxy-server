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

All traffic passes through the proxy, where filtering and logging are applied.

---

## Concurrency Model

-   **Thread-per-connection**
-   Each client connection is handled in a separate thread

---

## Request Handling

### HTTP Requests

1. Read request headers until `\r\n\r\n`
2. Parse request line and `Host` header
3. Check destination against blocklist
4. If allowed:
    - Forward request to destination server
    - Stream response back to client
5. Close connections

### HTTPS Requests (CONNECT)

1. Parse `CONNECT host:port`
2. Validate against blocklist
3. Establish TCP connection to destination
4. Send `200 Connection Established`
5. Forward bytes bidirectionally (full-duplex)

Encrypted traffic is not inspected.

---

## Blocklist

-   Loaded once at startup from a configuration file
-   Stored in memory using `std::set`
-   Case-insensitive matching
-   Read-only access ensures thread safety

Blocked requests receive `403 Forbidden`.

---

## Logging

Each request is logged with:

-   Timestamp
-   Destination host
-   Request line
-   Action (ALLOWED / BLOCKED)

Logging is thread-safe and flushed immediately.

---

## Conclusion

This design demonstrates core networking concepts including TCP socket programming, HTTP parsing, concurrency, and secure HTTPS tunneling in a clear and modular manner.
