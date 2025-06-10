# ChronoCache

ChronoCache is a high-performance C++14+ thread-safe caching library with time-based expiration (TTL).  
Designed with extensibility and performance in mind, it offers a solution for managing temporary key-value data in multi-threaded applications.

## Features

- **Thread-safe** design using standard C++ synchronization primitives.
- **TTL-based expiration** to automatically remove stale entries.
- **Modular architecture** to support additional eviction strategies like LRU and LFU.
- **Cache-friendly** memory layout and future-ready for lock-free implementations.

## Use Cases

- Session/token caching in authentication systems.
- Temporary storage in data processing pipelines.
- In-memory object lifecycles in microservices or high-performance backend systems.

## Build & Setup

### Requirements

- **C++14** or newer
- **CMake 3.10+**
- Optional: Google Benchmark and Google Test for performance and unit testing

### Clone and Build

```bash
git clone https://github.com/ldias83/ChronoCache.git
cd ChronoCache
mkdir build && cd build
cmake ..
make -j$(nproc)
```

## Example

```cpp
ChronoCache<std::string, std::string> cache;
cache.put("token_001", "123456789", std::chrono::seconds(5));

if (auto value = cache.get("token_001")) {
    std::cout << "Valid token: " << *value << "\n";
} else {
    std::cout << "Token expired or not found.\n";
}
