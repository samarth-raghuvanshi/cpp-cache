#include <string>
#include <fstream>
#include <filesystem>
#include "CacheSerializer.h"

bool CacheSerializer::save(const Cache &cache, const std::string &filename) {
    auto entries = cache.getEntries();
    std::filesystem::path path = "../data";
    std::filesystem::create_directories(path);
    path /= filename;
    if (!path.has_extension()) {
        path += ".db";
    }
    std::ofstream out(path);
    if(!out)
        return false;
    out << cache.getCapacity() << "\n";
    for (const auto &entry : entries) {
        out << entry.key << " " << entry.value << " " << entry.ttl << "\n";
    }
    return out.good();
}

bool CacheSerializer::load(Cache &cache, const std::string &filename) {
    std::filesystem::path path = "../data";
    path /= filename;
    if (!path.has_extension()) {
        path += ".db";
    }
    std::ifstream in(path);
    if (!in)
        return false;
    size_t capacity;
    if(!(in >> capacity))
        return false;
    cache.clear();
    cache.resize(capacity);
    std::string key, value;
    int ttl;
    while (in >> key >> value >> ttl) {
        if (ttl == -1)
            cache.set(key, value);
        else 
            cache.set(key, value, ttl);
    } 
    return in.eof();
}