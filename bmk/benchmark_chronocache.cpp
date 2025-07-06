
/**
 * @file benchmark_chronocache.cpp
 * @brief Google Benchmark suite for ChronoCache.
 *
 * This file contains performance benchmarks for the ChronoCache data structure,
 * measuring insertion, retrieval, and access patterns under various scenarios.
 *
 * Benchmark scenarios include:
 *  - bm_putSingle:     repeated insertion of a fixed key (overwrites previous value)
 *  - bm_getHit:        repeated retrieval of an existing key (cache hit)
 *  - bm_GetMiss:       repeated retrieval of a missing key (cache miss)
 *  - bm_getRandomKeys: randomized access across pre-filled keys (low locality)
 *
 * The results of each benchmark report average execution time per operation.
 *
 * Google Benchmark options such as --benchmark_filter and --benchmark_min_time
 * can be used to select and control execution from the command line.
 *
 * Author: Lucas Dias
 * Project: ChronoCache
 */


#include <benchmark/benchmark.h>
#include <optional>
#include <random>
#include "ChronoCache.hpp"

constexpr long int kBenchmarkIterationsDft {50'000'000};
constexpr long int kBenchmarkIterationsPut {20'000'000};
constexpr long int kBenchmarkIterationsRdm {100'000};


/// \brief Benchmark measuring the time to insert a single key-value pair into the cache.
/// 
/// Each iteration measures the cost of `put()` for the same key (42) and value (99),
/// overwriting the previous value each time. TTL is set to 1 second.

static void bm_putSingle(benchmark::State& state) {
    ChronoCache<int, int> cache;
    for (auto _ : state) {
        cache.put(42, 99, std::chrono::seconds(1));
    }
}
BENCHMARK(bm_putSingle)->Iterations(kBenchmarkIterationsDft);



/// \brief Stress benchmark for `ChronoCache::put()` mirroring the Valgrind run.
///
/// Inserts a fresh key (`key++`) each iteration with a long TTL (60 s), forcing
/// the map to grow without any expiry-driven de-allocations. Latency numbers
/// from Google Benchmark can be correlated 1-to-1 with allocation totals from
/// the Valgrind stress-test (see main.cpp), because the workloads are identical.
///
/// \note Keep the `static int key` counter; it guarantees monotonically
///       increasing keys across all iterations.

static void bm_putStress(benchmark::State& state) {
    ChronoCache<int, int> cache { static_cast<std::size_t>(kBenchmarkIterationsPut) };
    for (auto _ : state) {
        static int key = 0;
        cache.put(key++, 123, std::chrono::seconds(60));
    }
}
BENCHMARK(bm_putStress);



/// \brief Benchmark measuring the time to retrieve a key that is guaranteed to exist in 
///        the cache (Chrono cache hit).
///
/// The cache is pre-filled with a key (42). Each iteration retrieves it via `get()`,
/// simulating repeated access to the same cached item (high temporal locality).

static void bm_getHit(benchmark::State& state) {
    ChronoCache<int, int> cache;
    cache.put(42, 99, std::chrono::seconds(1));
    
    for (auto _ : state) {
        auto result = cache.get(42);
        if (result.has_value()) {
            benchmark::DoNotOptimize(result);
        }
    }
}
BENCHMARK(bm_getHit)->Iterations(kBenchmarkIterationsDft);



/// \brief Benchmark measuring the time to retrieve random keys from the cache.
///
/// The cache is pre-filled with 1000 entries. On each iteration, a random key from this range is accessed.
/// This simulates low temporal locality and tests performance under randomized access patterns.

static void bm_getRandomKeys(benchmark::State& state) {
    ChronoCache<int, int> cache;
    constexpr int N  { 1000 };
    std::vector<int> keys {};

    for (int i = 0; i < N; ++i)
        keys.push_back(i), cache.put(i, i * 2, std::chrono::seconds(5));

    std::mt19937 rng(42);
    std::uniform_int_distribution<int> dist(0, N - 1);

    for (auto _ : state) {
        auto key   { keys[dist(rng)] };
        auto value { cache.get(key)  };
        benchmark::DoNotOptimize(value.value_or(0));
    }
}
BENCHMARK(bm_getRandomKeys);



/// \brief Benchmark measuring the time to attempt retrieval of a non-existent key (cache miss).
///
/// The cache is left empty. Each iteration calls `get()` with a key that was never inserted,
/// allowing you to measure the best-case path for hash lookup and miss handling.

static void bm_GetMiss(benchmark::State& state) {
    ChronoCache<int, int> cache; // no insertion = miss
    for (auto _ : state) {
        auto result = cache.get(-1);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(bm_GetMiss)->Iterations(kBenchmarkIterationsDft);

BENCHMARK_MAIN();
