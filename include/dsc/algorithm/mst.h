#ifndef DSC_ALGORITHM_MST_H
#define DSC_ALGORITHM_MST_H

#include <vector>
#include <algorithm>
#include <limits>
#include <queue>
#include <utility>
#include "../disjoint_sets.h"

namespace dsc {
namespace algorithm {

// ============================================================
// 最小生成树（MST）算法
// ============================================================

const int INF_MST = std::numeric_limits<int>::max() / 2;

// 边结构：(权重, from, to)
using Edge = std::pair<int, std::pair<int, int>>;

// -------------------- Kruskal 算法 --------------------
// edges: 边列表，每条边为 (权重, (from, to))
// V: 顶点数量
// 返回 MST 的边列表
inline std::vector<Edge> Kruskal(std::vector<Edge> edges, int V) {
    // 按权重升序排序
    std::sort(edges.begin(), edges.end());

    dsc::RankDS ds(V);
    std::vector<Edge> mst;

    for (const auto& edge : edges) {
        int weight = edge.first;
        int u = edge.second.first;
        int v = edge.second.second;

        if (ds.find(u) != ds.find(v)) {
            ds.union_sets(u, v);
            mst.push_back(edge);
            if (static_cast<int>(mst.size()) == V - 1) break;
        }
    }
    return mst;
}

// -------------------- Prim 算法（优先队列优化）--------------------
// graph: 邻接矩阵 graph[u][v] = 权重，graph[u][v] = INF_MST 表示无边
// 返回 MST 的总权重
// 时间复杂度 O(E log V)，使用惰性删除避免 decrease_key
inline int Prim(const std::vector<std::vector<int>>& graph) {
    int V = static_cast<int>(graph.size());
    if (V == 0) return 0;

    std::vector<bool> in_mst(V, false);
    using Entry = std::pair<int, int>; // (weight, vertex)
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> pq;

    pq.push({0, 0}); // 从顶点 0 开始
    int total_weight = 0;

    while (!pq.empty()) {
        auto top = pq.top(); pq.pop();
        int w = top.first;
        int u = top.second;

        if (in_mst[u]) continue; // 惰性删除：跳过已在 MST 中的条目

        in_mst[u] = true;
        total_weight += w;

        for (int v = 0; v < V; ++v) {
            if (!in_mst[v] && graph[u][v] != INF_MST) {
                pq.push({graph[u][v], v});
            }
        }
    }
    return total_weight;
}

// -------------------- Prim 算法（无堆，O(V²)）--------------------
// 适用于稠密图
inline int PrimNaive(const std::vector<std::vector<int>>& graph) {
    int V = static_cast<int>(graph.size());
    if (V == 0) return 0;

    std::vector<int> key(V, INF_MST);     // key[i] = 连接到 MST 的最小边权
    std::vector<bool> in_mst(V, false);   // 是否已在 MST 中
    key[0] = 0;
    int total_weight = 0;

    for (int count = 0; count < V; ++count) {
        // 选取不在 MST 中且 key 最小的顶点
        int u = -1;
        int min_key = INF_MST;
        for (int i = 0; i < V; ++i) {
            if (!in_mst[i] && key[i] < min_key) {
                min_key = key[i];
                u = i;
            }
        }
        if (u == -1) break;  // 图不连通

        in_mst[u] = true;
        total_weight += key[u];

        // 更新相邻顶点的 key
        for (int v = 0; v < V; ++v) {
            if (!in_mst[v] && graph[u][v] != 0 && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
            }
        }
    }
    return total_weight;
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_MST_H
