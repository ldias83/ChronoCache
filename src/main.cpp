#include <iostream>
#include <thread>
#include <chrono>
#include "ChronoCache.hpp"

template <typename K, typename V>
void tryGetCache(ChronoCache<K, V>& cache, const K& key) {
    if (auto value = cache.get(key)) {
        //std::cout << "Found valid token: " << key << ": " << *value << std::endl;
    } else {
        //std::cout << "Expired or not valid token: " << key << std::endl;
    }
}

int main() {
    ChronoCache<int, int> cache;
    const int rounds   = 50'000'000;
    const int num_keys = 100'000;

    // Warm-up phase (just insert everything once)
    for (int i = 0; i < num_keys; ++i) {
        cache.put(i, i * 10, std::chrono::seconds(5));  // Long TTL to avoid expiration
    }

    auto start = std::chrono::steady_clock::now();

    // Stress test: hit `get()` and `put()` in loop
    for (int i = 0; i < rounds; ++i) {
        int key = i % num_keys;

        cache.get(key); // mostly hits
        cache.put(key, key * 20, std::chrono::seconds(5));  // overwrite with same TTL
    }

    return 0;
}