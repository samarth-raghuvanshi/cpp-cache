#include <iostream>
#include "Cache.h"

int main() {
    Cache ch;
    ch.set("name", "cppcache");
    auto s = ch.get("name");
    if (s.has_value())
        std::cout << s.value() << std::endl;
    else
        std::cout << "Key not found" << std::endl;
    return 0;
}


