#include "Cache.h"

Cache::Cache(size_t capacity) : capacity(capacity) {}

int Cache::resize(size_t newCapacity) {
    capacity = newCapacity;
    int counter = 0;
    while(lruList.size()>capacity) {
        evict();
        counter+=1;
    }
    return counter;
}

size_t Cache::getCapacity() const {
    return capacity;
}

size_t Cache::getSize() const {
    return lruList.size();
}

void Cache::set(const std::string &key, const std::string &value) {
    if (capacity==0)
        return;
    const auto it = cacheMap.find(key);
    if (it!=cacheMap.end()) {
        it->second.value = value;
        touch(it->second);
    }
    else {
        if(lruList.size()==capacity)
            evict();
        lruList.emplace_front(key);
        cacheMap.emplace(key, Entry{value, lruList.begin()});
    }
}

std::optional<std::string> Cache::get(const std::string &key) {
    const auto it = cacheMap.find(key);
    if (it == cacheMap.end())
        return std::nullopt;
    touch(it->second);
    return it->second.value;
}

void Cache::erase(const std::string &key) {
    const auto it = cacheMap.find(key);
    if (it != cacheMap.end()) {
        lruList.erase(it->second.itr);
        cacheMap.erase(it);
    }
}

bool Cache::exists(const std::string &key) {
    const auto it = cacheMap.find(key);
    if (it==cacheMap.end())
        return false;
    touch(it->second);
    return true;
}

void Cache::touch(Entry &entry) {
    lruList.splice(lruList.begin(), lruList, entry.itr);
}

void Cache::evict() {
    cacheMap.erase(lruList.back());
    lruList.pop_back();
}