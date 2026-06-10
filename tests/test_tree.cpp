#include <dsc/tree.h>
#include <iostream>
#include <cassert>
#include <vector>

std::vector<char> result;
void record(const char& v) { result.push_back(v); }

int main() {
    // 括号表达式构造
    dsc::Tree<char> t1("A(B,C(D,E))");

    result.clear();
    t1.pre_order_rec(record);
    assert(result == std::vector<char>({'A', 'B', 'C', 'D', 'E'}));

    result.clear();
    t1.post_order_rec(record);
    assert(result == std::vector<char>({'B', 'D', 'E', 'C', 'A'}));

    // 层序遍历
    result.clear();
    t1.level_order(record);
    assert(result == std::vector<char>({'A', 'B', 'C', 'D', 'E'}));

    // 双亲表示法构造
    // A(0, -1), B(1, 0), C(2, 0)
    std::vector<std::pair<char, int>> parents = {
        {'A', -1}, {'B', 0}, {'C', 0}
    };
    dsc::Tree<char> t2(parents);
    result.clear();
    t2.pre_order_rec(record);
    assert(result == std::vector<char>({'A', 'B', 'C'}));

    // 非递归遍历
    result.clear();
    t1.pre_order_nor(record);
    assert(result == std::vector<char>({'A', 'B', 'C', 'D', 'E'}));

    result.clear();
    t1.post_order_nor(record);
    assert(result == std::vector<char>({'B', 'D', 'E', 'C', 'A'}));

    // 拷贝
    dsc::Tree<char> t3(t1);
    result.clear();
    t3.level_order(record);
    assert(result == std::vector<char>({'A', 'B', 'C', 'D', 'E'}));

    std::cout << "test_tree: all tests passed!" << std::endl;
    return 0;
}
