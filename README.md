# cpp-cache

A lightweight, in-memory key-value cache written in modern C++17. It features an interactive command-line interface, LRU eviction, optional per-key TTL expiration, and dynamic capacity resizing.

## Features

- **In-memory storage** for fast key-value lookups
- **LRU eviction** when the cache reaches its capacity
- **Optional TTL** on keys with `SET ... TTL <seconds>`, `EXPIRE`, `PERSIST`, and `TTL` commands
- **Dynamic resizing** with automatic eviction of least-recently-used entries
- **Interactive CLI** with colorized output and built-in help

## Building

This project uses CMake and requires a C++17-compatible compiler.

```bash
# Configure
cmake -B build

# Build
cmake --build build
```

## Running

```bash
./build/cpp-cache
```

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
| `INFO` | Print current capacity, entry count, and usage |
| `HELP` | Display command usage |
| `EXIT` | Quit the program |

## Project Structure

```
cpp-cache/
├── CMakeLists.txt
├── include/
│   ├── Cache.h
│   ├── CommandParser.h
│   └── Color.h
├── src/
│   ├── main.cpp
│   ├── Cache.cpp
│   └── CommandParser.cpp
└── build/
```
