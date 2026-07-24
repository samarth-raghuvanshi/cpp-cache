#include "Cache.h"
#include <chrono>

Cache::Cache(size_t capacity) : capacity(capacity) {}

void Cache::set(const std::string &key, const std::string &value) {
    if (capacity==0)
        return;
    auto it = cacheMap.find(key);
    if (it != cacheMap.end() && isExpired(it->second)) {
        erase(it);
        it = cacheMap.end();
    }
    if (it!=cacheMap.end()) {
        it->second.value = value;
        it->second.hasExpiry = false;
        touch(it->second);
    }
    else {
        if(lruList.size()==capacity)
            evict();
        lruList.emplace_front(key);
        cacheMap.emplace(key, Entry{value, lruList.begin()});
    }
}

void Cache::set(const std::string &key, const std::string &value, const int ttl) {
    if (capacity==0)
        return;
    auto it = cacheMap.find(key);
    auto expiry = std::chrono::steady_clock::now()+std::chrono::seconds(ttl);
    if (it != cacheMap.end() && isExpired(it->second)) {
        erase(it);
        it = cacheMap.end();
    }
    if (it!=cacheMap.end()) {
        it->second.value = value;
        it->second.hasExpiry = true;
        it->second.expiry = expiry;
        touch(it->second);
    }
    else {
        if(lruList.size()==capacity)
            evict();
        lruList.emplace_front(key);
        cacheMap.emplace(key, Entry{value, lruList.begin(), true, expiry});
    }
}

std::optional<std::string> Cache::get(const std::string &key) {
    const auto it = cacheMap.find(key);
    if (it == cacheMap.end())
        return std::nullopt;
    if (isExpired(it->second)) {
        erase(it);
        return std::nullopt;
    }
    touch(it->second);
    return it->second.value;
}

bool Cache::eraseKey(const std::string &key) {
    const auto it = cacheMap.find(key);
    if (it != cacheMap.end()) {
        erase(it);
        return true;
    }
    return false;
}

bool Cache::exists(const std::string &key) {
    auto it = cacheMap.find(key);
    if (it==cacheMap.end())
        return false;
    if (isExpired(it->second)) {
        erase(it);
        return false;
    }
    touch(it->second);
    return true;
}

int Cache::resize(size_t newCapacity) {
    capacity = newCapacity;
    int counter = 0;
    while(lruList.size()>capacity) {
        evict();
        counter+=1;
    }
    return counter;
}

bool Cache::setExpiry(const std::string &key, int ttl) {
    auto it = cacheMap.find(key);
    if (it == cacheMap.end())
        return false;
    if (isExpired(it->second)) {
        erase(it);
        return false;
    }
    it->second.hasExpiry = true;
    it->second.expiry = std::chrono::steady_clock::now() + std::chrono::seconds(ttl);
    touch(it->second);
    return true;
}

bool Cache::persist(const std::string &key) {
    auto it = cacheMap.find(key);
    if (it == cacheMap.end())
        return false;
    if (isExpired(it->second)) {
        erase(it);
        return false;
    }
    it->second.hasExpiry = false;
    touch(it->second);
    return true;
}

int Cache::getTTL(const std::string &key) {
    auto it = cacheMap.find(key);
    if (it == cacheMap.end())
        return -2;
    if (isExpired(it->second)) {
        erase(it);
        return -2;
    }
    if (!it->second.hasExpiry) {
        return -1;
    }
    auto remaining = std::chrono::duration_cast<std::chrono::seconds>(it->second.expiry - std::chrono::steady_clock::now()).count();
    return static_cast<int>(std::max(0LL, remaining));
}

void Cache::cleanupExpired() {
    for (auto it = cacheMap.begin(); it != cacheMap.end();) {
        if (isExpired(it->second)) {
            auto current = it++;
            erase(current);
        }
        else
            ++it;
    }
}

size_t Cache::getCapacity() const {
    return capacity;
}

size_t Cache::getSize() const {
    return lruList.size();
}

bool Cache::isExpired(const Entry& entry) const
{
    return entry.hasExpiry &&
           std::chrono::steady_clock::now() >= entry.expiry;
}

void Cache::touch(Entry &entry) {
    lruList.splice(lruList.begin(), lruList, entry.itr);
}

void Cache::evict() {
    cacheMap.erase(lruList.back());
    lruList.pop_back();
}

void Cache::erase(std::unordered_map<std::string, Entry>::iterator it) {
    lruList.erase(it->second.itr);
    cacheMap.erase(it);
}