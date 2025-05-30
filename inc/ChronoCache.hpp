#include <chrono>
#include <string>
#include <unordered_map>

using Clock = std::chrono::steady_clock;

class ChronoCache {
public:

    auto put(const std::string& key, const std::string& value, std::chrono::seconds ttl) -> void {
        auto expiration { Clock::now() + ttl };
        m_cache[key] = { value, expiration };
    }

    auto get(const std::string& key) -> std::optional<std::string> {
        if (m_cache.find(key) != m_cache.end()) {
            auto& [value, expiration] = m_cache[key];
            if (Clock::now() < expiration) {
                return value;
            }
            m_cache.erase(key);
        }
        return std::nullopt;
    }

private:
    struct Entry {
        std::string value;
        Clock::time_point expiry;
    };

    std::unordered_map<std::string, Entry> m_cache {};
};