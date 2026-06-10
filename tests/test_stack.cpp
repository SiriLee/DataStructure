#include <dsc/stack.h>
#include <iostream>
#include <cassert>

int main() {
    dsc::Stack<int> s;
    assert(s.empty());

    s.push(1);
    s.push(2);
    s.push(3);
    assert(s.size() == 3);
    assert(s.top() == 3);

    s.pop();
    assert(s.top() == 2);
    assert(s.size() == 2);

    s.clear();
    assert(s.empty());

    // 拷贝
    dsc::Stack<int> s2;
    s2.push(10);
    dsc::Stack<int> s3(s2);
    assert(s3.top() == 10);

    // 移动
    dsc::Stack<int> s4(std::move(s3));
    assert(s4.top() == 10 && s3.empty());

    std::cout << "test_stack: all tests passed!" << std::endl;
    return 0;
}
