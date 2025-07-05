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

## Performance Reports & Release Notes

Every tagged release ships with an **evidence-based report** in **`rpt/`**
that:

1. **Profiles micro-architectural behavior** (VTune, `perf`, Massif).  
2. **Benchmarks core operations** with Google Benchmark.  
3. **Identifies bottlenecks** and **justifies each subsequent change**.  
4. Provides a **quantitative baseline** so regressions are obvious.

| Release | Report | Highlights |
| ------- | ------ | ---------- |
| **v1.0.00** | [`rpt/v1.0.00.pdf`](rpt/v1.0.00.pdf) | Functional baseline, naïve `std::unordered_map`, single mutex   |
| **v1.0.01** | [`rpt/v1.0.01.pdf`](rpt/v1.0.01.pdf) | Minor refactor, allocator hot-spot analysis, roadmap formalised |

*Why bother?*  
By coupling every code delta with a focused report, contributors can reason
about **“why this change?”** rather than **“what changed?”**—accelerating code
review, validating optimisations, and documenting trade-offs for future
maintainers.


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

## License

© 2025 Lucas Dias – MIT License. See the [LICENSE](LICENSE) file for details.

