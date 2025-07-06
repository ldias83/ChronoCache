#include "ChronoCache.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>

// Stop the optimiser from deleting results.
template <typename T>
inline void keep(const T& v) { asm volatile("" : : "g"(v) : "memory"); }

// Constants 
constexpr std::size_t ITERS     { 15'000'000 };                // total ops
constexpr auto        TTL_LONG  { std::chrono::seconds(60) };  // never expires
constexpr auto        TTL_SHORT { std::chrono::seconds(01) };  // expires instantly 


void stressPut(ChronoCache<int,int>& cache, std::size_t begin) {

    for (std::size_t i { begin }; i < ITERS; ++i) {
        cache.put(i, i + 42, TTL_LONG);
    }
}

int main() {

    ChronoCache<int,int> cache { static_cast<std::size_t>(ITERS) };
    int key { 1 };

    // key 0: never inserted (covers cold miss path) 
    // key 1: always-hit 
    // key 2: expires immediately
    cache.put(key++, 0, TTL_LONG);
    cache.put(key++, 1, TTL_SHORT);

    stressPut(cache, key);

    for (std::size_t i {}; i < ITERS; ++i) {

        // cycles 0,1,2,3,0,1,2,3…
        switch (i & 3) {

            case 0: keep(cache.get(0));         break;  // cold miss
            case 1: keep(cache.get(1));         break;  // guaranteed hit
            case 2: keep(cache.get(2));         break;  // usually expired
            case 3: cache.put(2, 1, TTL_SHORT); break;  // re-insert key-2
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
}