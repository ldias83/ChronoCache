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

    ChronoCache<int, std::string> cache;
    const int rounds { 10'000'000 };

    // Insert lots of keys with short TTLs
    for (int i = 0; i < rounds; ++i) {
        int key = i % 1000;
        std::string value = std::to_string(i * 12345);
        cache.put(key, value, std::chrono::seconds(3));
    }

    // Sleep to let some TTLs expire
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // Try to get them all — some will hit, most will miss
    for (int i = 0; i < rounds; ++i) {
        int key = i % 1000;
        tryGetCache(cache, key);
    }

    return 0;
}