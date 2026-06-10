#include <dsc/algorithm/rolling_hash.h>
#include <iostream>
#include <cassert>

int main() {
    using namespace dsc::algorithm;

    // 单哈希
    {
        RollingHash rh("hello world");
        assert(rh.find("world") == 6);
        assert(rh.find("hello") == 0);
        assert(rh.find("xyz") == static_cast<std::size_t>(-1));

        // 子串哈希
        long long h1 = rh.get_hash(0, 4);  // "hello"
        RollingHash rh2("hello");
        assert(h1 == rh2.get_hash());
    }

    // 双哈希
    {
        RollingHashPlus rh("hello world");
        assert(rh.find("world") == 6);
        assert(rh.find("hello") == 0);
    }

    std::cout << "test_rolling_hash: all tests passed!" << std::endl;
    return 0;
}
