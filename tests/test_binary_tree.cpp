#include <dsc/binary_tree.h>
#include <iostream>
#include <cassert>
#include <vector>

std::vector<int> result;
void record(const int& v) { result.push_back(v); }

int main() {
    dsc::BinaryTree<int> bt;
    assert(bt.empty());

    bt.set_root(1);
    assert(!bt.empty());
    assert(bt.root() == 1);

    bt.insert_left(1, 2);
    bt.insert_right(1, 3);
    bt.insert_left(2, 4);
    bt.insert_right(2, 5);

    // 先序递归
    result.clear();
    bt.pre_order_rec(record);
    assert(result == std::vector<int>({1, 2, 4, 5, 3}));

    // 中序递归
    result.clear();
    bt.in_order_rec(record);
    assert(result == std::vector<int>({4, 2, 5, 1, 3}));

    // 后序递归
    result.clear();
    bt.post_order_rec(record);
    assert(result == std::vector<int>({4, 5, 2, 3, 1}));

    // 层序遍历
    result.clear();
    bt.level_order(record);
    assert(result == std::vector<int>({1, 2, 3, 4, 5}));

    // 非递归先序
    result.clear();
    bt.pre_order_nor(record);
    assert(result == std::vector<int>({1, 2, 4, 5, 3}));

    // 非递归中序
    result.clear();
    bt.in_order_nor(record);
    assert(result == std::vector<int>({4, 2, 5, 1, 3}));

    // 非递归后序
    result.clear();
    bt.post_order_nor(record);
    assert(result == std::vector<int>({4, 5, 2, 3, 1}));

    // 高度
    assert(bt.height() == 3);
    assert(bt.size() == 5);

    // 拷贝比较
    dsc::BinaryTree<int> bt2(bt);
    assert(bt == bt2);

    std::cout << "test_binary_tree: all tests passed!" << std::endl;
    return 0;
}
