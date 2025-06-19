#include "ChronoCache.hpp"
#include <iostream>
#include <thread>
#include <chrono>
#include <numeric>      // iota
#include <random>
#include <vector>
#include <algorithm>

/* Stop the optimiser from deleting results. */
template <typename T>
inline void keep(const T& v) { asm volatile("" : : "g"(v) : "memory"); }

/* ─── quick knobs ─────────────────────────────────────────── */
constexpr std::size_t ITERS = 400'000'000;                // total ops
constexpr auto  TTL_LONG    = std::chrono::seconds(60);   // never expires
constexpr auto  TTL_SHORT   = std::chrono::seconds(01);   // expires instantly 
/* ─────────────────────────────────────────────────────────── */

int main()
{
    ChronoCache<int,int> cache;

    // key 0: always-hit       | key 1: expires immediately
    // key 2: never inserted   | (covers cold miss path) 
    cache.put(0, 0, TTL_LONG);
    cache.put(1, 1, TTL_SHORT);

    for (std::size_t i = 0; i < ITERS; ++i)
    {
        switch (i & 3)          // cycles 0,1,2,3,0,1,2,3…
        {
            case 0: keep(cache.get(0));         break;  // guaranteed hit
            case 1: keep(cache.get(1));         break;  // usually expired
            case 2: keep(cache.get(2));         break;  // cold miss
            case 3: cache.put(1, 1, TTL_SHORT); break;  // re-insert key-1
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
}