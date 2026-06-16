// ============================================================
// test_rb_tree.cpp — 红黑树测试
// ============================================================

#include <dsc/rb_tree.h>
#include <iostream>
#include <cassert>

void test_insert_search() {
    dsc::RBTree t;

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
    dsc::RBTree t;

    t.insert(5);
    t.insert(5);  // 重复不应插入
    t.insert(5);

    // 树中应只有一个5
    assert(t.search(5));

    std::cout << "  duplicate insert: OK\n";
}

void test_empty() {
    dsc::RBTree t;

    assert(!t.search(0));

    std::cout << "  empty tree: OK\n";
}

void test_large_sequential() {
    dsc::RBTree t;
    const int N = 100;

    for (int i = 0; i < N; ++i) {
        t.insert(i);
    }

    for (int i = 0; i < N; ++i) {
        assert(t.search(i));
    }

    std::cout << "  large sequential (N=100): OK\n";
}

void test_large_descending() {
    // 逆序插入，测试右倾插入的平衡
    dsc::RBTree t;
    const int N = 100;

    for (int i = N - 1; i >= 0; --i) {
        t.insert(i);
    }

    for (int i = 0; i < N; ++i) {
        assert(t.search(i));
    }

    std::cout << "  large descending (N=100): OK\n";
}

int main() {
    std::cout << "=== test_rb_tree ===\n";

    test_insert_search();
    test_duplicate();
    test_empty();
    test_large_sequential();
    test_large_descending();

    std::cout << "\ntest_rb_tree: all tests passed!\n";
    return 0;
}
