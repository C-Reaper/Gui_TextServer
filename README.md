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
make -f Makefile.web all  # For Webassembly for Emscripten or wasmtime
```

To execute the application, use:

```sh
make -f Makefile.linux exe  # For Linux
make -f Makefile.windows exe  # For Windows
make -f Makefile.wine exe  # For Linux cross compile for Windows
make -f Makefile.web exe  # For Webassembly for Emscripten or wasmtime
```

---

### Additional Information

- **Client**: The client application can be executed directly from the `build` directory.
- **Server**: The server application requires interaction via a command line interface to start listening and handling connections.

This README provides a clear guide on how to build and run the project, ensuring that all necessary steps are covered for each target platform.