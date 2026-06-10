#include <dsc/queue.h>
#include <iostream>
#include <cassert>

int main() {
    dsc::Queue<int> q;
    assert(q.empty());

    q.push(1);
    q.push(2);
    q.push(3);
    assert(q.size() == 3);
    assert(q.front() == 1);
    assert(q.back() == 3);

    q.pop();
    assert(q.front() == 2);
    assert(q.size() == 2);

    q.push(4);
    assert(q.back() == 4);

    while (!q.empty()) q.pop();
    assert(q.empty());

    // 环形行为测试
    dsc::Queue<int> q2(3);  // 初始容量 3
    q2.push(1);
    q2.push(2);
    q2.pop();
    q2.push(3);
    q2.push(4);  // 触发扩容
    assert(q2.size() == 3);
    assert(q2.front() == 2);

    std::cout << "test_queue: all tests passed!" << std::endl;
    return 0;
}
