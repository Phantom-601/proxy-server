# Testing Document – HTTP/HTTPS Proxy Server

This document describes the test cases and commands used to verify the correct behavior of the proxy server, including request forwarding, domain blocking, concurrent client handling, and malformed request handling.

All tests were performed on **Windows** using `curl` and the command prompt.

---

## 1. Successful Forwarding of Requests

### Objective

Verify that the proxy correctly forwards HTTP requests to destination servers and relays responses back to the client.

### Command

```bat
curl -x localhost:8888 http://example.co
```

### Expected Behavior

-   The HTML content of `example.com` is returned to the client.
-   The proxy establishes a connection to the destination server.
-   An entry is written to the log file indicating `ALLOWED`.

### Verification

-   Response content is displayed in terminal.
-   `logs/proxy.log` contains:

```bat
2026-01-04 03:07:05 | example.com | GET http://example.com/ HTTP/1.1 | ALLOWED
```

## 2. Blocking of Blacklisted Domains

### Objective

Verify that requests to blocked domains are denied by the proxy.

### Setup

Add the domain to the blocklist:

```text
    super.com
```

Restart the proxy server.

### Command

```terminal
    curl -x localhost:8888 http://super.com
```

### Expected Behaviour

-   Client Receives :
    ```text
    HTTP/1.1 403 Forbidden
    ```
-   No connection is made to the destination server.
-   The request is logged as BLOCKED.

### Verification

logs/proxy.log contains:

```text
2026-01-04 03:14:17 | example.com | GET http://super.com/ HTTP/1.1 | BLOCKED

```

## 3. HTTPS (CONNECT) Request Handling

### Objective

Verify correct handling of HTTPS requests using the CONNECT method.

### Command

```terminal
curl -x localhost:8888 https://www.google.com
```

### Expected Behaviour

-   The proxy establishes a TCP tunnel using CONNECT.
-   Encrypted HTTPS traffic is forwarded transparently.
-   The client successfully receives the response.

### Verification

-   No proxy errors or crashes.
-   Log entry indicates ALLOWED for the CONNECT request.
    ```text
    2026-01-08 21:34:02 | www.google.com:443 | CONNECT www.google.com:443 HTTP/1.1
    | ALLOWED
    ```

## 4. Concurrent Client Handling

### Objective

Verify that the proxy can handle multiple clients simultaneously.

### Method

Open multiple terminals and run the following command concurrently:

```terminal
curl -x localhost:8888 http://example.com
```

### Expected Behaviour

-   All requests are served without blocking.

-   Proxy remains responsive.

-   Each request is handled independently.

###

-   Multiple log entries appear in logs/proxy.log.

-   No request waits for another to complete.

## 5. Handling of Malformed Requests

### Objective

Verify that the proxy safely handles malformed or incomplete HTTP requests.

### Command

```terminal
curl -x localhost:8888 http:///
```

### Expected Behaviour

-   Proxy does not crash.

-   Connection is closed gracefully.

-   No undefined behavior occurs.

### Verification

-   Proxy continues running.

-   No invalid entries are written to the log file.
