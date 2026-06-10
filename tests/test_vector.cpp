#include <dsc/vector.h>
#include <iostream>
#include <cassert>

int main() {
    // 默认构造
    dsc::Vector<int> v1;
    assert(v1.empty() && v1.size() == 0);

    // push_back
    v1.push_back(1);
    v1.push_back(2);
    v1.push_back(3);
    assert(v1.size() == 3);
    assert(v1[0] == 1 && v1[1] == 2 && v1[2] == 3);

    // initializer_list 构造
    dsc::Vector<int> v2 = {5, 4, 3, 2, 1};
    assert(v2.size() == 5);

    // at
    assert(v2.at(0) == 5);
    bool caught = false;
    try { v2.at(100); } catch (const std::out_of_range&) { caught = true; }
    assert(caught);

    // front / back
    assert(v2.front() == 5);
    assert(v2.back() == 1);

    // pop_back
    v2.pop_back();
    assert(v2.size() == 4 && v2.back() == 2);

    // insert
    v2.insert(1, 99);
    assert(v2[1] == 99 && v2.size() == 5);

    // erase
    v2.erase(1);
    assert(v2[1] == 4 && v2.size() == 4);

    // clear
    v2.clear();
    assert(v2.empty());

    // 拷贝构造
    dsc::Vector<int> v3(v1);
    assert(v3.size() == 3 && v3[0] == 1 && v3[1] == 2 && v3[2] == 3);

    // 移动构造
    dsc::Vector<int> v4(std::move(v3));
    assert(v4.size() == 3 && v3.empty());

    // resize
    dsc::Vector<int> v5 = {1, 2, 3};
    v5.resize(5, 0);
    assert(v5.size() == 5 && v5[3] == 0 && v5[4] == 0);

    // reserve / capacity
    v5.reserve(100);
    assert(v5.capacity() >= 100);
    assert(v5.size() == 5);  // reserve 不改变 size

    // shrink_to_fit
    v5.shrink_to_fit();
    assert(v5.capacity() == 5);

    // swap
    dsc::Vector<int> a = {1, 2};
    dsc::Vector<int> b = {3, 4, 5};
    a.swap(b);
    assert(a.size() == 3 && b.size() == 2);

    std::cout << "test_vector: all tests passed!" << std::endl;
    return 0;
}
