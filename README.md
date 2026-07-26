# cpp-cache

A lightweight, in-memory key-value cache written in modern C++17. It features an interactive command-line interface, TCP client/server mode, LRU eviction, optional per-key TTL expiration, dynamic capacity resizing, and persistent storage.

## Features

- **In-memory storage** for fast key-value lookups
- **LRU eviction** when the cache reaches its capacity
- **Optional TTL** on keys with `SET ... TTL <seconds>`, `EXPIRE`, `PERSIST`, and `TTL` commands
- **Dynamic resizing** with automatic eviction of least-recently-used entries
- **Persistent storage** via `SAVE` and `LOAD` commands (stored in `data/`)
- **Interactive CLI** with built-in help
- **TCP client/server mode** for remote cache access over the network

## Building

This project uses CMake and requires a C++17-compatible compiler. It builds four executables: `cli`, `server`, `client`, and `benchmark`.

```bash
# Configure
cmake -B build

# Build
cmake --build build
```

## Running

### Interactive CLI

```bash
./build/cli
```

### Server

```bash
./build/server
```

The server listens on port `9000` by default.

### Client

```bash
./build/client
```

Enter the server address and port when prompted. Leave the address blank to use `127.0.0.1`.

## Commands

| Command | Description |
| --- | --- |
| `SET <key> <value> [TTL <seconds>]` | Store a key-value pair, optionally with expiration |
| `GET <key>` | Retrieve the value at `key` |
| `EXISTS <key>` | Check if `key` exists in the cache |
| `DELETE <key>` | Remove `key` from the cache |
| `RESIZE <capacity>` | Change the cache capacity |
| `EXPIRE <key> <seconds>` | Set or update TTL for an existing key |
| `PERSIST <key>` | Remove expiration from an existing key |
| `TTL <key>` | Show remaining seconds until expiration |
| `SAVE <filename>` | Save cache entries to `data/<filename>.db` |
| `LOAD <filename>` | Load cache entries from `data/<filename>.db` |
| `CLEAR` | Remove all entries from the cache |
| `INFO` | Print current capacity, entry count, and usage |
| `HELP` | Display command usage |
| `EXIT` | Quit the program |

## Project Structure

```
cpp-cache/
├── CMakeLists.txt
├── include/
│   ├── Cache.h
│   ├── CacheSerializer.h
│   ├── Client.h
│   ├── CommandParser.h
│   └── Server.h
├── src/
│   ├── Cache.cpp
│   ├── CacheSerializer.cpp
│   ├── Client.cpp
│   ├── CommandParser.cpp
│   └── Server.cpp
├── apps/
│   ├── benchmark_main.cpp
│   ├── cli_main.cpp
│   ├── client_main.cpp
│   └── server_main.cpp
├── data/
└── build/
```

## Benchmark

A benchmark executable is included to measure core cache operations.

```bash
./build/benchmark.exe
```

| Operation | Median | Average |
| --- | --- | --- |
| SET | 943.78 ns | 947.14 ns |
| GET | 557.26 ns | 558.28 ns |
| EXISTS | 501.65 ns | 502.20 ns |
| DELETE | 700.72 ns | 707.62 ns |
