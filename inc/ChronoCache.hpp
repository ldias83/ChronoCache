#include <chrono>
#include <string>
#include <unordered_map>

using Clock = std::chrono::steady_clock;

template <typename K, typename V>
class ChronoCache {
public:
    explicit ChronoCache(std::size_t capacity = 0) {
        if (capacity > 0) {
            m_cache.reserve(capacity);
        }
    }
    
    auto put(const K& key, const V& value, std::chrono::seconds ttl) -> void {
        auto expiration { Clock::now() + ttl };
        m_cache[key] = { value, expiration };
    }

    [[nodiscard]]
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

    [[nodiscard]]
    auto size() const -> std::size_t {
        std::size_t n {};
        const auto now { Clock::now() };
        for (const auto& [_, entry] : m_cache) {
            n = (now < entry.expiry) ? n + 1 : n;
        }
        return n;
    }

private:
    struct Entry {
        V value;
        Clock::time_point expiry;
    };

    std::unordered_map<K, Entry> m_cache {};
};