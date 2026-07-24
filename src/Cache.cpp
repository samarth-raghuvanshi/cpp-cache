#include "Cache.h"

void Cache::set(const std::string &key, const std::string &value) {
            cacheMap[key] = value;
}
std::optional<std::string> Cache::get(const std::string &key) const {
            const auto it = cacheMap.find(key);
            if(it == cacheMap.end())
                return std::nullopt;
            return it->second;
}
void Cache::erase(const std::string &key) {
            cacheMap.erase(key);
}
bool Cache::exists(const std::string &key) const {
            if(cacheMap.find(key)==cacheMap.end())
                return false;
            return true;
}

        
