# kv-store

A simplified in-memory key-value store, built from scratch in C++ as a learning project — think a minimal Redis/Memcached.

## Status: work in progress

Currently implemented:
- In-memory hash table storage (`Store` class: get/set/del)
- TCP server with a simple text protocol
- Multi-threaded (one thread per client connection)

Planned:
- Sharded concurrency model (benchmarked against a single global mutex)
- Persistence (append-only log)
- LRU eviction policy

## Building

Requires CMake and a C++17 compiler.

\`\`\`bash
mkdir build && cd build
cmake ..
make
./kv_store
\`\`\`

## Usage

Server listens on port 6380. Connect with `nc` or any raw TCP client:

\`\`\`
nc localhost 6380
SET foo bar
GET foo
DEL foo
\`\`\`

## Protocol

Plain-text, newline-terminated commands:
- `SET <key> <value>` → `OK`
- `GET <key>` → value, or `(nil)` if missing
- `DEL <key>` → `1` if removed, `0` if key didn't exist