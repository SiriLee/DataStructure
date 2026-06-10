#ifndef DSC_DISJOINT_SETS_H
#define DSC_DISJOINT_SETS_H

#include <vector>
#include <cstddef>

namespace dsc {

// ============================================================
// DisjointSets — 并查集（Union-Find）
// 支持路径压缩和按秩合并
// ============================================================
class DisjointSets {
public:
    // 构造 n 个互不相交的集合 {0}, {1}, ..., {n-1}
    explicit DisjointSets(int n);

    // 查找 x 所属集合的代表元素（带路径压缩）
    int find(int x);

    // 合并 x 和 y 所在的集合（按秩合并）
    void union_sets(int x, int y);

private:
    std::vector<int> parent_;  // 父节点，根节点的父节点为自身
    std::vector<int> rank_;    // 秩（近似高度）
};

// ==================== 实现 ====================

inline DisjointSets::DisjointSets(int n) : parent_(n), rank_(n, 0) {
    for (int i = 0; i < n; ++i) {
        parent_[i] = i;
    }
}

inline int DisjointSets::find(int x) {
    // 路径压缩：将 x 到根的路径上所有节点直接连到根
    if (parent_[x] != x) {
        parent_[x] = find(parent_[x]);
    }
    return parent_[x];
}

inline void DisjointSets::union_sets(int x, int y) {
    int root_x = find(x);
    int root_y = find(y);
    if (root_x == root_y) return;

    // 按秩合并：将秩较小的树连到秩较大的树下
    if (rank_[root_x] < rank_[root_y]) {
        parent_[root_x] = root_y;
    } else if (rank_[root_x] > rank_[root_y]) {
        parent_[root_y] = root_x;
    } else {
        parent_[root_y] = root_x;
        ++rank_[root_x];
    }
}

} // namespace dsc

#endif // DSC_DISJOINT_SETS_H
