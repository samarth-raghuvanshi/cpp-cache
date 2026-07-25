#pragma once
#include "Cache.h"
#include <string>

class CacheSerializer {
    public:
        static bool save(const Cache &cache, const std::string &filename);
        static bool load(Cache &cache, const std::string &filename);
};