## Project README

### Overview
This project is a simple TCP client-server application designed for communication between a client and a server. The client can send messages to the server, which then broadcasts these messages to all connected clients.

### Features
- **Client**: Connects to a server on port 5000 and sends/receives messages.
- **Server**: Listens on port 5000 for incoming connections and broadcasts received messages to all connected clients.

## Project Structure

### Prerequisites
- C/C++ Compiler (GCC)
- Make utility

## Build & Run

To build the project, navigate to the project directory and run:

```sh
make -f Makefile.linux all  # For Linux
make -f Makefile.windows all  # For Windows
make -f Makefile.wine all  # For Linux cross compile for Windows
make -f Makefile.web all  # Native server plus Emscripten browser client
```

The server address and port are build settings shared by all targets; no source
edits are needed when moving to another host:

```sh
make -f Makefile.web all SERVER_HOST=192.168.1.20 SERVER_PORT=5000
make -f Makefile.linux all SERVER_HOST=192.168.1.20 SERVER_PORT=5000
```

`SERVER_HOST` is the TCP server address from the client's point of view (for
WebAssembly, from the machine running the POSIX proxy).
The makefiles locate the shared `System` library at `../../../System` by
default; use `SYSTEM_ROOT=/path/to/System` if your checkout has a different
layout.

For WebAssembly, `all` builds the native TCP server and the browser client. Run
the server in one terminal:

```sh
make -f Makefile.web exe
```

Then run the browser client in another terminal:

```sh
make -f Makefile.web client
```

The `client` target builds and starts Emscripten's POSIX WebSocket proxy
automatically; the browser client initializes its connection through the
platform-neutral networking API. The proxy forwards TCP socket operations to
the machine running the proxy, where the native server is listening on port
5000. `emrun` serves the page with the cross-origin isolation headers required
by WebAssembly threads. No Python helper is needed.

By default the browser connects to `ws://localhost:8081`, which is suitable
when the browser and proxy run on the same machine. If they are on separate
machines, set `WEB_PROXY_URL` to a WebSocket URL reachable from the browser and
run the proxy on the machine that can reach the TCP server. Start it in one
terminal:

```sh
make -f Makefile.web proxy
```

Serve the browser page in a second terminal, pointing it at the proxy:

```sh
make -f Makefile.web client-page WEB_PROXY_URL=ws://192.168.1.20:8081
```

Use `wss://` behind a TLS-enabled reverse proxy when serving the page over
HTTPS. The POSIX proxy grants browser code TCP access from the machine running
it; keep it on a trusted network and do not expose it publicly without suitable
access controls.

Event messages use a length-prefixed, big-endian wire format and are buffered
until a complete frame arrives, so clients built for different CPU
architectures can communicate over the same TCP server. Rebuild both client
and server when adopting this format; older event-protocol binaries are not
wire-compatible.

To build the Linux, Windows, or Wine versions, use:

```sh
make -f Makefile.linux exe
make -f Makefile.windows exe
make -f Makefile.wine exe
```

---

### Additional Information

- **Client**: The client application can be executed directly from the `build` directory.
- **Server**: The server application requires interaction via a command line interface to start listening and handling connections.

This README provides a clear guide on how to build and run the project, ensuring that all necessary steps are covered for each target platform.