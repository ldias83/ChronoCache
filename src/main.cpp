#include <iostream>
#include <thread>
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

    std::string token {"token_001"};
    std::string value {"123456789"};
    ChronoCache<std::string, std::string> cache;

    cache.put(token, value, std::chrono::seconds(5));
    tryGetCache(cache, token);
    
    std::this_thread::sleep_for(std::chrono::seconds(5));
    tryGetCache(cache, token);

    return 0;
}