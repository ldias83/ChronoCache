#include <iostream>
#include <thread>
#include "ChronoCache.hpp"

template <typename T>
void tryGetCache(ChronoCache& cache, const T& key) {
    if (auto value = cache.get(key)) {
        std::cout << "Found valid token: " << key << ": " << *value << std::endl;
    } else {
        std::cout << "Expired or not valid token: " << key << std::endl;
    }
}

int main() {

    ChronoCache cache;
    cache.put("token_001", "123456", std::chrono::seconds(5));
    tryGetCache(cache, "token_001");
    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    tryGetCache(cache, "token_001");

    return 0;
}