#include <dsc/algorithm/kmp.h>
#include <iostream>
#include <cassert>

int main() {
    using namespace dsc::algorithm;

    // 基本匹配
    assert(KmpSearch("hello world", "world") == 6);
    assert(KmpSearch("hello world", "hello") == 0);
    assert(KmpSearch("hello world", "abc") == -1);

    // 空模式
    assert(KmpSearch("hello", "") == 0);

    // 修正版 KMP
    assert(KmpSearchFixed("abababc", "ababc") == 2);
    assert(KmpSearchFixed("aaaaa", "aa") == 0);

    // 重复字符
    assert(KmpSearch("aaaaa", "aaa") == 0);

    // next 数组
    {
        auto next = GetNext("ababc");
        assert(next.size() == 5);
    }
    {
        auto next = GetFixedNext("ababc");
        assert(next.size() == 5);
    }

    std::cout << "test_kmp: all tests passed!" << std::endl;
    return 0;
}
