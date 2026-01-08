# Testing Document – HTTP/HTTPS Proxy Server

This document describes the test cases and commands used to verify the correct behavior of the proxy server, including request forwarding, domain blocking, concurrent client handling, and malformed request handling.

All tests were performed on **Windows** using `curl` and the command prompt.

---

## 1. Successful Forwarding of Requests

### Objective

Verify that the proxy correctly forwards HTTP requests to destination servers and relays responses back to the client.

### Command

````bat
curl -x localhost:8888 http://example.co
## Expected Behavior

- The HTML content of `example.com` is returned to the client.
- The proxy establishes a connection to the destination server.
- An entry is written to the log file indicating `ALLOWED`.

## Verification

- Response content is displayed in terminal.
- `logs/proxy.log` contains:

```sql
<timestamp> | example.com | GET http://example.com/ HTTP/1.1 | ALLOWED
````
