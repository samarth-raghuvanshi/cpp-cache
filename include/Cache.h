#pragma once
#include <unordered_map>
#include <string>
#include <optional>
#include <list>
#include <chrono>
#include <vector>

struct Entry {
    std::string value;
    std::list<std::string>::iterator itr;
    bool hasExpiry = false;
    std::chrono::steady_clock::time_point expiry;
};

struct dbEntry {
    std::string key;
    std::string value;
    int ttl;
};

class Cache {
    public: 
        explicit Cache(size_t capacity);
        void set(const std::string &key, const std::string &value);
        void set(const std::string &key, const std::string &value, const int ttl);
        std::optional<std::string> get(const std::string &key);
        bool eraseKey(const std::string &key);
        bool exists(const std::string &key);
        int resize(size_t newCapacity);
        bool setExpiry(const std::string &key, int ttl);
        bool persist(const std::string &key);
        int getTTL(const std::string &key);
        size_t getCapacity() const;
        size_t getSize() const;
        void cleanupExpired();
        void clear();
        std::vector<dbEntry> getEntries() const;
    private:
        bool isExpired(const Entry& entry) const;
        void touch(Entry &entry);
        void evict();
        void erase(std::unordered_map<std::string, Entry>::iterator it);
        int toSeconds(const Entry &entry) const;
        size_t capacity;
        std::list<std::string> lruList;
        std::unordered_map<std::string, Entry> cacheMap;
};