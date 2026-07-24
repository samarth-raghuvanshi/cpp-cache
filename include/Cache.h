#pragma once
#include <unordered_map>
#include <string>
#include <optional>
#include <list>

struct Entry {
    std::string value;
    std::list<std::string>::iterator itr;
};

class Cache {
    public: 
        explicit Cache(size_t capacity);
        void set(const std::string &key, const std::string &value);
        std::optional<std::string> get(const std::string &key);
        void erase(const std::string &key);
        bool exists(const std::string &key);
        int resize(size_t newCapacity);
        size_t getCapacity() const;
        size_t getSize() const;
    private:
        void touch(Entry &entry);
        void evict();
        size_t capacity;
        std::list<std::string> lruList;
        std::unordered_map<std::string, Entry> cacheMap;
};