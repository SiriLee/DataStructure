// ============================================================
// test_avl_tree.cpp — AVL树测试
// ============================================================

#include <dsc/avl_tree.h>
#include <iostream>
#include <cassert>

void test_insert_search() {
    dsc::AVLTree t;

    t.insert(5);
    t.insert(3);
    t.insert(8);
    t.insert(1);
    t.insert(4);

    assert(t.search(5));
    assert(t.search(3));
    assert(t.search(8));
    assert(t.search(1));
    assert(t.search(4));
    assert(!t.search(99));
    assert(!t.search(0));

    std::cout << "  insert & search: OK\n";
}

void test_duplicate() {
    dsc::AVLTree t;

    t.insert(5);
    t.insert(5);  // 重复不应插入
    t.insert(5);

    // 树中应只有一个5，删除一次后应不存在
    t.remove(5);
    assert(!t.search(5));

    std::cout << "  duplicate insert: OK\n";
}

void test_remove() {
    dsc::AVLTree t;

    t.insert(5);
    t.insert(3);
    t.insert(8);
    t.insert(1);
    t.insert(4);
    t.insert(7);
    t.insert(9);

    // 删除叶子
    t.remove(1);
    assert(!t.search(1));
    assert(t.search(3));

    // 删除只有一个子节点的节点
    t.remove(8);
    assert(!t.search(8));
    assert(t.search(9));  // 9 应仍在

    // 删除有两个子节点的节点
    t.remove(5);
    assert(!t.search(5));
    assert(t.search(3));
    assert(t.search(4));
    assert(t.search(7));
    assert(t.search(9));

    std::cout << "  remove: OK\n";
}

void test_remove_nonexistent() {
    dsc::AVLTree t;
    t.insert(1);
    t.insert(2);

    t.remove(99);  // 不应崩溃
    assert(t.search(1));
    assert(t.search(2));

    std::cout << "  remove nonexistent: OK\n";
}

void test_empty() {
    dsc::AVLTree t;

    assert(!t.search(0));
    t.remove(0);  // 空树删除不应崩溃

    std::cout << "  empty tree: OK\n";
}

void test_rotate_left() {
    // 顺序插入 1,2,3 触发左旋 (RR)
    dsc::AVLTree t;
    t.insert(1);
    t.insert(2);
    t.insert(3);

    assert(t.search(1));
    assert(t.search(2));
    assert(t.search(3));

    std::cout << "  rotate left (1,2,3): OK\n";
}

void test_rotate_right() {
    // 逆序插入 3,2,1 触发右旋 (LL)
    dsc::AVLTree t;
    t.insert(3);
    t.insert(2);
    t.insert(1);

    assert(t.search(1));
    assert(t.search(2));
    assert(t.search(3));

    std::cout << "  rotate right (3,2,1): OK\n";
}

void test_rotate_left_right() {
    // 插入 3,1,2 触发左右双旋 (LR)
    dsc::AVLTree t;
    t.insert(3);
    t.insert(1);
    t.insert(2);

    assert(t.search(1));
    assert(t.search(2));
    assert(t.search(3));

    std::cout << "  rotate left-right (3,1,2): OK\n";
}

void test_rotate_right_left() {
    // 插入 1,3,2 触发右左双旋 (RL)
    dsc::AVLTree t;
    t.insert(1);
    t.insert(3);
    t.insert(2);

    assert(t.search(1));
    assert(t.search(2));
    assert(t.search(3));

    std::cout << "  rotate right-left (1,3,2): OK\n";
}

void test_large_sequential() {
    // 大量顺序插入，验证自平衡
    dsc::AVLTree t;
    const int N = 100;

    for (int i = 0; i < N; ++i) {
        t.insert(i);
    }

    for (int i = 0; i < N; ++i) {
        assert(t.search(i));
    }

    // 删除一半
    for (int i = 0; i < N; i += 2) {
        t.remove(i);
    }

    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0) {
            assert(!t.search(i));
        } else {
            assert(t.search(i));
        }
    }

    std::cout << "  large sequential (N=100): OK\n";
}

int main() {
    std::cout << "=== test_avl_tree ===\n";

    test_insert_search();
    test_duplicate();
    test_remove();
    test_remove_nonexistent();
    test_empty();
    test_rotate_left();
    test_rotate_right();
    test_rotate_left_right();
    test_rotate_right_left();
    test_large_sequential();

    std::cout << "\ntest_avl_tree: all tests passed!\n";
    return 0;
}
