#ifndef DSC_ALGORITHM_EXTERNAL_SORT_H
#define DSC_ALGORITHM_EXTERNAL_SORT_H

#include <vector>
#include <queue>
#include <algorithm>
#include <cassert>

namespace dsc {
namespace algorithm {

// ============================================================
// 外部排序算法（int，升序）
// 1. 置换-选择排序 → 产生初始归并段
// 2. 最佳归并树 + k-路归并 → 反复合并最小的 k 个段
// ============================================================

// ==================== 置换-选择排序 ====================

// 用优先队列产生若干初始归并段。
// pq 存 (组号, 数据)，新记录 < 上次输出则归入下一组（冻结）。
inline std::vector<std::vector<int>> ReplacementSelection(
        const std::vector<int>& input, int buffer_size) {
    
    int n = static_cast<int>(input.size());
    if (n == 0) return {};

    int M = buffer_size;

    // 数据量不超过缓冲区，直接排序返回单段
    if (n <= M) {
        std::vector<int> result = input;
        std::sort(result.begin(), result.end());
        return {result};
    }

    // 小顶堆，按 (组号, 数据) 升序
    using Entry = std::pair<int, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

    for (int i = 0; i < M; ++i) pq.push({0, input[i]});

    int input_pos   = M; // 下一个输入位置
    int current_run = 0; // 当前组号

    std::vector<int>              cur; // 当前段
    std::vector<std::vector<int>> runs; // 已完成的段

    while (!pq.empty()) {
        auto [group, val] = pq.top(); pq.pop();
        // 组号变大，说明当前段完成
        if (group > current_run) {
            runs.push_back(std::move(cur));
            cur.clear();
            current_run = group;
        }
        // 输出当前数据
        cur.push_back(val);
        // 读入下一数据
        if (input_pos < n) {
            int next = input[input_pos++];
            int new_group = (next < val) ? current_run + 1 : current_run;
            pq.push({new_group, next});
        }
    }

    if (!cur.empty()) runs.push_back(std::move(cur));
    return runs;
}

// ==================== k-路归并 ====================

// 用优先队列合并 k 个有序段。
inline std::vector<int> KWayMerge(const std::vector<std::vector<int>>& runs) {
    int k = static_cast<int>(runs.size());
    assert(k > 0);

    // 小顶堆：(数据, 段索引)
    using Entry = std::pair<int, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;
    std::vector<int> pos(k, 0); // 每段当前访问位置
    // 将每个段的第一个元素入堆
    for (int i = 0; i < k; ++i)
        if (pos[i] < static_cast<int>(runs[i].size()))
            pq.push({runs[i][pos[i]++], i});

    std::vector<int> result;

    while (!pq.empty()) {
        auto [val, idx] = pq.top(); pq.pop();
        result.push_back(val); // 输出当前最小元素
        // 将同一段的下一个元素入堆
        if (pos[idx] < static_cast<int>(runs[idx].size()))
            pq.push({runs[idx][pos[idx]++], idx});
    }

    return result;
}

// ==================== 完整外部排序 ====================

// 置换-选择产生初始段后，反复合并最小的 k 个段（最佳归并树策略），直到只剩一个有序结果。
inline std::vector<int> ExternalSort(
        const std::vector<int>& data, int buffer_size, int k) {
    // 1. 置换-选择排序 → 产生初始归并段
    auto runs = ReplacementSelection(data, buffer_size);
    if (runs.size() <= 1)
        return runs.empty() ? std::vector<int>{} : std::move(runs[0]);

    int n = static_cast<int>(runs.size());

    // 2. 最佳归并树 + k-路归并 → 反复合并最小的 k 个段
    // 补长度 0 的虚拟段
    int d = (k - 1 - (n - 1) % (k - 1)) % (k - 1);
    for (int i = 0; i < d; ++i) runs.push_back({});
    // 小顶堆：(段长, 段索引)，每次取最小的 k 个合并
    using Entry = std::pair<int, int>;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;
    for (int i = 0; i < static_cast<int>(runs.size()); ++i)
        pq.push({static_cast<int>(runs[i].size()), i});

    while (pq.size() > 1) {
        // 取长度最小的 k 个段
        std::vector<std::vector<int>> to_merge;
        for (int i = 0; i < k; ++i) {
            auto [len, idx] = pq.top(); pq.pop();
            if (idx >= 0 && !runs[idx].empty())
                to_merge.push_back(std::move(runs[idx]));
        }
        // 合并得到一个新段，添加到 runs 尾部并放回堆中
        runs.push_back(KWayMerge(to_merge));
        pq.push({static_cast<int>(runs.back().size()),
                 static_cast<int>(runs.size()) - 1});
    }

    return std::move(runs.back());
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_EXTERNAL_SORT_H
