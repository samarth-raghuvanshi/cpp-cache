#pragma once
#include <unordered_map>
#include <string>
#include <optional>

class Cache {
    public: 
        void set(const std::string &key, const std::string &value);
        std::optional<std::string> get(const std::string &key) const;
        void erase(const std::string &key);
        bool exists(const std::string &key) const;
    private:
        std::unordered_map<std::string, std::string> cacheMap;
};
