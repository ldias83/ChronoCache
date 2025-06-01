#include <iostream>
#include <thread>
#include <chrono>
#include "ChronoCache.hpp"

template <typename K, typename V>
void tryGetCache(ChronoCache<K, V>& cache, const K& key) {
    if (auto value = cache.get(key)) {
        std::cout << "Found valid token: " << key << ": " << *value << std::endl;
    } else {
        std::cout << "Expired or not valid token: " << key << std::endl;
    }
}

int main() {

    ChronoCache<std::string, std::string> cache;
    const int rounds { 10000 };

    // Insert lots of keys with short TTLs
    for (int i = 0; i < rounds; ++i) {
        cache.put("token_" + std::to_string(i), std::to_string(i * 12345), std::chrono::seconds(3));
    }

    // Sleep to let some TTLs expire
    std::this_thread::sleep_for(std::chrono::milliseconds(120));

    // Try to get them all — some will hit, most will miss
    for (int i = 0; i < rounds; ++i) {
        tryGetCache(cache, "token_" + std::to_string(i));
    }


    return 0;
}