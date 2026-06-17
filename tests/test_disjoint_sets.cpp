#include <dsc/disjoint_sets.h>
#include <iostream>
#include <cassert>

void test_compress_ds() {
    dsc::CompressDS ds(5);

    // Initially all disjoint
    assert(ds.find(0) != ds.find(1));

    ds.union_sets(0, 1);
    assert(ds.find(0) == ds.find(1));

    ds.union_sets(1, 2);
    assert(ds.find(0) == ds.find(2));

    ds.union_sets(3, 4);
    assert(ds.find(3) == ds.find(4));
    assert(ds.find(0) != ds.find(3));

    // Merge two sets
    ds.union_sets(2, 3);
    assert(ds.find(0) == ds.find(4));

    // Self-union should be safe
    ds.union_sets(0, 0);
    assert(ds.find(0) == ds.find(1));

    std::cout << "test_compress_ds: all tests passed!" << std::endl;
}

void test_rank_ds() {
    dsc::RankDS ds(5);

    // Initially all disjoint
    assert(ds.find(0) != ds.find(1));

    ds.union_sets(0, 1);
    assert(ds.find(0) == ds.find(1));

    ds.union_sets(1, 2);
    assert(ds.find(0) == ds.find(2));

    ds.union_sets(3, 4);
    assert(ds.find(3) == ds.find(4));
    assert(ds.find(0) != ds.find(3));

    // Merge two sets
    ds.union_sets(2, 3);
    assert(ds.find(0) == ds.find(4));

    // Self-union should be safe
    ds.union_sets(0, 0);
    assert(ds.find(0) == ds.find(1));

    std::cout << "test_rank_ds: all tests passed!" << std::endl;
}

int main() {
    test_compress_ds();
    test_rank_ds();
    return 0;
}
