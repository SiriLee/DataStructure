#include <dsc/btree.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <algorithm>
#include <random>

// ================================================================
// Helpers
// ================================================================

/// Insert all keys and assert every Insert returns true (no duplicates).
template<int m>
void insert_all(dsc::BTree<m>& tree, const std::vector<int>& keys) {
    for (int k : keys) {
        assert(tree.Insert(k) && "Insert should succeed for new key");
    }
}

/// Verify Search finds every key and returns false for missing keys.
template<int m>
void verify_search(const dsc::BTree<m>& tree, const std::vector<int>& present,
                   const std::vector<int>& absent) {
    for (int k : present) {
        assert(tree.Search(k) && "Search should find inserted key");
    }
    for (int k : absent) {
        assert(!tree.Search(k) && "Search should return false for missing key");
    }
}

/// Generate sequential vector [start, end).
std::vector<int> seq(int start, int end) {
    std::vector<int> v;
    for (int i = start; i < end; ++i) v.push_back(i);
    return v;
}

// ================================================================
// Tests — m = 3
// ================================================================

void test_empty_m3() {
    dsc::BTree<3> tree;
    assert(tree.Empty());
    assert(!tree.Search(0));
    assert(!tree.Search(42));
    std::cout << "test_empty_m3: passed!" << std::endl;
}

void test_single_element_m3() {
    dsc::BTree<3> tree;
    assert(tree.Insert(10));
    assert(!tree.Empty());
    assert(tree.Search(10));
    assert(!tree.Search(5));
    assert(!tree.Search(15));
    // Duplicate
    assert(!tree.Insert(10));
    std::cout << "test_single_element_m3: passed!" << std::endl;
}

void test_ordered_insert_m3() {
    dsc::BTree<3> tree;
    auto keys = seq(0, 10);
    insert_all(tree, keys);
    verify_search(tree, keys, { -5, 100, 999 });
    std::cout << "test_ordered_insert_m3: passed!" << std::endl;
}

void test_reverse_insert_m3() {
    dsc::BTree<3> tree;
    std::vector<int> keys;
    for (int i = 20; i >= 0; --i) keys.push_back(i);
    insert_all(tree, keys);
    verify_search(tree, keys, { -1, 21, 99 });
    std::cout << "test_reverse_insert_m3: passed!" << std::endl;
}

void test_leaf_split_m3() {
    // m=3: max 2 keys per node. Inserting 3 keys triggers leaf split.
    dsc::BTree<3> tree;
    insert_all(tree, {1, 2, 3});  // 3rd insert overflows leaf
    assert(tree.Search(1));
    assert(tree.Search(2));
    assert(tree.Search(3));
    assert(!tree.Search(0));
    assert(!tree.Search(4));
    std::cout << "test_leaf_split_m3: passed!" << std::endl;
}

void test_root_split_m3() {
    // Insert enough keys to trigger root split
    dsc::BTree<3> tree;
    auto keys = seq(0, 10);
    insert_all(tree, keys);
    verify_search(tree, keys, { -1, 10, 100 });
    std::cout << "test_root_split_m3: passed!" << std::endl;
}

void test_multi_level_split_m3() {
    // Insert many keys to trigger cascading splits up to root level
    dsc::BTree<3> tree;
    auto keys = seq(0, 100);
    insert_all(tree, keys);
    verify_search(tree, keys, { -5, 100, 200, 500 });

    // Also test reverse order insertion into fresh tree
    dsc::BTree<3> tree2;
    for (int i = 99; i >= 0; --i) {
        assert(tree2.Insert(i));
    }
    auto keys2 = seq(0, 100);
    verify_search(tree2, keys2, { -1, 100, 200 });
    std::cout << "test_multi_level_split_m3: passed!" << std::endl;
}

void test_duplicate_rejection_m3() {
    dsc::BTree<3> tree;
    insert_all(tree, {5, 3, 7, 1, 9});
    // Try re-inserting each
    assert(!tree.Insert(5));
    assert(!tree.Insert(3));
    assert(!tree.Insert(7));
    assert(!tree.Insert(1));
    assert(!tree.Insert(9));
    // Still findable
    assert(tree.Search(5));
    assert(tree.Search(3));
    assert(tree.Search(7));
    assert(tree.Search(1));
    assert(tree.Search(9));
    std::cout << "test_duplicate_rejection_m3: passed!" << std::endl;
}

void test_random_insert_m3() {
    dsc::BTree<3> tree;
    std::vector<int> keys;
    for (int i = -50; i <= 50; ++i) keys.push_back(i);

    // Shuffle deterministically
    std::mt19937 rng(42);
    std::shuffle(keys.begin(), keys.end(), rng);

    insert_all(tree, keys);
    // Verify all keys found
    for (int k : keys) {
        assert(tree.Search(k));
    }
    // Missing neighbours
    assert(!tree.Search(-100));
    assert(!tree.Search(100));
    std::cout << "test_random_insert_m3: passed!" << std::endl;
}

// ================================================================
// Tests — m = 4 (even order)
// ================================================================

void test_m4_basic() {
    dsc::BTree<4> tree;
    assert(tree.Empty());

    auto keys = seq(0, 30);
    insert_all(tree, keys);
    verify_search(tree, keys, { -1, 30, 99 });

    // Duplicate rejection
    for (int k : keys) {
        assert(!tree.Insert(k));
    }
    std::cout << "test_m4_basic: passed!" << std::endl;
}

// ================================================================
// Tests — m = 5 (larger order, deeper splits)
// ================================================================

void test_m5_many_keys() {
    dsc::BTree<5> tree;
    auto keys = seq(0, 200);
    insert_all(tree, keys);
    verify_search(tree, keys, { -1, 200, 500 });

    // Reverse order into fresh tree
    dsc::BTree<5> tree2;
    for (int i = 199; i >= 0; --i) {
        assert(tree2.Insert(i));
    }
    verify_search(tree2, keys, { -1, 200 });
    std::cout << "test_m5_many_keys: passed!" << std::endl;
}

void test_m5_duplicates() {
    dsc::BTree<5> tree;
    insert_all(tree, {10, 20, 30, 40, 50, 5, 15, 25});
    assert(!tree.Insert(10));
    assert(!tree.Insert(30));
    assert(!tree.Insert(50));
    assert(!tree.Insert(5));
    // Verify all still present
    for (int k : {5, 10, 15, 20, 25, 30, 40, 50}) {
        assert(tree.Search(k));
    }
    std::cout << "test_m5_duplicates: passed!" << std::endl;
}

// ================================================================
// Tests — m = 2 (minimum order)
// ================================================================

void test_m2_min_order() {
    dsc::BTree<2> tree;
    assert(tree.Empty());

    auto keys = seq(0, 20);
    insert_all(tree, keys);
    verify_search(tree, keys, { -1, 20, 99 });

    assert(!tree.Insert(0));
    assert(!tree.Insert(10));
    std::cout << "test_m2_min_order: passed!" << std::endl;
}

// ================================================================
// Tests — Edge cases
// ================================================================

void test_alternating_insert() {
    // Insert alternating high/low to stress tree balance
    dsc::BTree<3> tree;
    std::vector<int> keys;
    for (int i = 0; i < 30; ++i) {
        keys.push_back(i);
        keys.push_back(100 - i);
    }
    insert_all(tree, keys);
    verify_search(tree, keys, { 200, -1 });
    std::cout << "test_alternating_insert: passed!" << std::endl;
}

void test_same_value_check() {
    // Multiple attempts on the same key before any insert
    dsc::BTree<3> tree;
    tree.Insert(42);
    assert(!tree.Insert(42));
    assert(!tree.Insert(42));
    assert(tree.Search(42));
    // Only one key
    assert(!tree.Search(41));
    assert(!tree.Search(43));
    std::cout << "test_same_value_check: passed!" << std::endl;
}

// ================================================================
// Main
// ================================================================

int main() {
    // m = 3
    test_empty_m3();
    test_single_element_m3();
    test_ordered_insert_m3();
    test_reverse_insert_m3();
    test_leaf_split_m3();
    test_root_split_m3();
    test_multi_level_split_m3();
    test_duplicate_rejection_m3();
    test_random_insert_m3();

    // m = 4
    test_m4_basic();

    // m = 5
    test_m5_many_keys();
    test_m5_duplicates();

    // m = 2
    test_m2_min_order();

    // Edge cases
    test_alternating_insert();
    test_same_value_check();

    std::cout << "test_btree: all tests passed!" << std::endl;
    return 0;
}
