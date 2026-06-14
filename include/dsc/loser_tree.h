#ifndef DSC_LOSER_TREE_H
#define DSC_LOSER_TREE_H

#include <vector>
#include <cassert>

namespace dsc {

// ============================================================
// 败者树（最小胜者树，升序）
// 叶子为选手，内部节点存败者索引，tree_[0] 为最终胜者。
// ============================================================
template<typename T>
struct LoserTree {
    int              k_;
    std::vector<T>   players_;
    std::vector<int> tree_;

    explicit LoserTree(int k);
    void Build(const std::vector<T>& players);
    int Winner() const;
    void Replay(int p);
};

// ==================== 败者树实现 ====================
// 构造函数，初始化 k 个选手和 k 个内部节点
template<typename T>
LoserTree<T>::LoserTree(int k) : k_(k), players_(k), tree_(k) {
    assert(k >= 1);
}

// 构建败者树，初始化所有内部节点为哨兵（k_），从最后一个叶子开始重赛
template<typename T>
void LoserTree<T>::Build(const std::vector<T>& players) {
    players_ = players;
    for (int i = 0; i < k_; ++i) tree_[i] = k_; // 哨兵，尚无败者
    for (int i = k_ - 1; i >= 0; --i) Replay(i); // 从最后一个叶子开始重赛
}

// 返回当前胜者索引-最小的选手
template<typename T>
int LoserTree<T>::Winner() const {
    return tree_[0];
}

// 重赛选手 p，沿路径向上比较并更新败者，直到根节点
template<typename T>
void LoserTree<T>::Replay(int p) {
    int t = (p + k_) / 2; // p 的父节点索引
    while (t > 0) {
        int loser = tree_[t]; // 当前败者索引
        if (loser == k_) { // 尚无败者
            tree_[t] = p; // p 预设为败者
            return;
        }
        // 败者更优则败者晋级，p 留下；否则 p 继续晋级
        if (players_[loser] < players_[p])
            std::swap(p, tree_[t]); // 胜者败者互换
        t /= 2; // 向上移动到父节点
    }
    tree_[0] = p; // 根节点存最终胜者索引
}

} // namespace dsc

#endif // DSC_LOSER_TREE_H
