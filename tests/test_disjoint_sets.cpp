#include <dsc/disjoint_sets.h>
#include <iostream>
#include <cassert>

int main() {
    dsc::DisjointSets ds(5);

    // 初始各自独立
    assert(ds.find(0) != ds.find(1));

    ds.union_sets(0, 1);
    assert(ds.find(0) == ds.find(1));

    ds.union_sets(1, 2);
    assert(ds.find(0) == ds.find(2));

    ds.union_sets(3, 4);
    assert(ds.find(3) == ds.find(4));
    assert(ds.find(0) != ds.find(3));

    // 合并两个集合
    ds.union_sets(2, 3);
    assert(ds.find(0) == ds.find(4));

    std::cout << "test_disjoint_sets: all tests passed!" << std::endl;
    return 0;
}
