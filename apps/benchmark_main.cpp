#include "Cache.h"
#include <chrono>
#include <iostream>
#include <string>
#include <vector>

constexpr int OPERATIONS = 1'000'000;

std::string key(int i) {
    return "key" + std::to_string(i);
}

std::string value(int i) {
    return "value" + std::to_string(i);
}

template <typename Func>
void benchmark(const std::string &name, Func &&func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();

    auto totalNs = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    double avgNs = static_cast<double>(totalNs) / OPERATIONS;

    std::cout << name << ":\n";
    std::cout << "  Total time: " << totalNs / 1'000'000.0 << " ms\n";
    std::cout << "  Average time per operation: " << avgNs << " ns\n\n";
}

int main() {
    Cache cache(OPERATIONS);

    std::vector<std::string> keys;
    std::vector<std::string> values;
    keys.reserve(OPERATIONS);
    values.reserve(OPERATIONS);

    for (int i = 0; i < OPERATIONS; ++i) {
        keys.push_back(key(i));
        values.push_back(value(i));
    }

    benchmark("SET", [&cache, &keys, &values]() {
        for (int i = 0; i < OPERATIONS; ++i) {
            cache.set(keys[i], values[i]);
        }
    });

    benchmark("GET", [&cache, &keys]() {
        for (int i = 0; i < OPERATIONS; ++i) {
            cache.get(keys[i]);
        }
    });

    benchmark("EXISTS", [&cache, &keys]() {
        for (int i = 0; i < OPERATIONS; ++i) {
            cache.exists(keys[i]);
        }
    });

    benchmark("DELETE", [&cache, &keys]() {
        for (int i = 0; i < OPERATIONS; ++i) {
            cache.eraseKey(keys[i]);
        }
    });

    return 0;
}
