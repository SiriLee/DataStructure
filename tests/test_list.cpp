#include <dsc/list.h>
#include <iostream>
#include <cassert>

int main() {
    // 默认构造
    dsc::List<int> l1;
    assert(l1.empty() && l1.size() == 0);

    // push_back
    l1.push_back(1);
    l1.push_back(2);
    l1.push_back(3);
    assert(l1.size() == 3);
    assert(l1.front() == 1 && l1.back() == 3);

    // push_front
    l1.push_front(0);
    assert(l1.front() == 0 && l1.size() == 4);

    // pop_front / pop_back
    l1.pop_front();
    assert(l1.front() == 1);
    l1.pop_back();
    assert(l1.back() == 2 && l1.size() == 2);

    // insert
    l1.insert(1, 99);
    assert(l1.size() == 3);
    // 顺序: 1, 99, 2
    l1.pop_front(); assert(l1.front() == 99);
    l1.pop_front(); assert(l1.front() == 2);

    // erase
    dsc::List<int> l2;
    l2.push_back(10);
    l2.push_back(20);
    l2.push_back(30);
    l2.erase(1);
    assert(l2.size() == 2 && l2.front() == 10 && l2.back() == 30);

    // 拷贝构造
    dsc::List<int> l3(l2);
    assert(l3.size() == 2 && l3.front() == 10);

    // 移动构造
    dsc::List<int> l4(std::move(l3));
    assert(l4.size() == 2 && l3.empty());

    // clear
    l4.clear();
    assert(l4.empty());

    // 赋值
    l4 = l2;
    assert(l4.size() == 2);

    // swap
    dsc::List<int> a, b;
    a.push_back(100); b.push_back(200);
    a.swap(b);
    assert(a.front() == 200 && b.front() == 100);

    std::cout << "test_list: all tests passed!" << std::endl;
    return 0;
}
