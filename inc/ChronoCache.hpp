#include <chrono>
#include <string>
#include <unordered_map>

using Clock = std::chrono::steady_clock;

template <typename K, typename V>
class ChronoCache {
public:
    auto put(const K& key, const V& value, std::chrono::seconds ttl) -> void {
        auto expiration { Clock::now() + ttl };
        m_cache[key] = { value, expiration };
    }

    auto get(const K& key) -> std::optional<V> {
        auto cacheIt { m_cache.find(key) };
        if (cacheIt != m_cache.end()) {
            auto& [value, expiration] = cacheIt->second;
            if (Clock::now() < expiration) {
                return value;
            }
            m_cache.erase(cacheIt);
        }
        return std::nullopt;
    }

private:
    struct Entry {
        V value;
        Clock::time_point expiry;
    };

    std::unordered_map<K, Entry> m_cache {};
};