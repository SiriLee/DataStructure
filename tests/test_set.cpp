#include <dsc/set.h>
#include <iostream>
#include <cassert>

int main() {
    dsc::Set<int> s;
    assert(s.empty());

    assert(s.insert(5));
    assert(s.insert(3));
    assert(s.insert(8));
    assert(s.insert(1));
    assert(!s.insert(5));  // 重复插入应失败
    assert(s.size() == 4);

    assert(s.contains(3));
    assert(!s.contains(99));

    assert(s.min() == 1);
    assert(s.max() == 8);

    s.erase(3);
    assert(!s.contains(3));
    assert(s.size() == 3);

    // 拷贝
    dsc::Set<int> s2(s);
    assert(s2.size() == 3);
    assert(s2.contains(5));

    // 移动
    dsc::Set<int> s3(std::move(s2));
    assert(s3.size() == 3 && s2.empty());

    s.clear();
    assert(s.empty());

    std::cout << "test_set: all tests passed!" << std::endl;
    return 0;
}
