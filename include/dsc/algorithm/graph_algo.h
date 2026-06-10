#ifndef DSC_ALGORITHM_GRAPH_ALGO_H
#define DSC_ALGORITHM_GRAPH_ALGO_H

#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

namespace dsc {
namespace algorithm {

// ============================================================
// AOV/AOE 网络算法
// - 拓扑排序 (Topological Sort) — AOV 网
// - 关键路径 (Critical Path) — AOE 网
// ============================================================

const int INF_CP = std::numeric_limits<int>::max() / 2;

// -------------------- 拓扑排序 --------------------
// graph: 邻接矩阵 graph[u][v] 表示 u->v 有边（权重任意，只检查是否有边）
// 返回拓扑序列，如果存在环则返回空序列
inline std::vector<int> TopologicalSort(
    const std::vector<std::vector<int>>& graph) {

    int V = static_cast<int>(graph.size());
    std::vector<int> in_degree(V, 0);

    // 计算入度
    for (int u = 0; u < V; ++u) {
        for (int v = 0; v < V; ++v) {
            if (graph[u][v] != 0 && u != v) {
                ++in_degree[v];
            }
        }
    }

    // 入度为 0 的顶点入队
    std::queue<int> q;
    for (int i = 0; i < V; ++i) {
        if (in_degree[i] == 0) {
            q.push(i);
        }
    }

    std::vector<int> result;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.push_back(u);

        for (int v = 0; v < V; ++v) {
            if (graph[u][v] != 0 && u != v) {
                --in_degree[v];
                if (in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }
    }

    // 如果有环（结果数量 != V），返回空
    if (static_cast<int>(result.size()) != V) {
        return {};
    }
    return result;
}

// -------------------- 关键路径（AOE 网） --------------------
// graph: 邻接矩阵 graph[u][v] = 活动持续时间，0 表示无边
// 返回整个工程的最早完成时间（关键路径长度）
// 同时返回每个顶点的最早开始时间 ve 和最迟开始时间 vl
inline int CriticalPath(
    const std::vector<std::vector<int>>& graph,
    std::vector<int>& ve,   // 输出：最早开始时间
    std::vector<int>& vl) { // 输出：最迟开始时间

    int V = static_cast<int>(graph.size());
    ve.assign(V, 0);
    vl.assign(V, INF_CP);

    // 先做拓扑排序
    auto topo = TopologicalSort(graph);
    if (topo.empty()) return -1;  // 存在环

    // 正推：计算 ve（最早开始时间）
    for (int u : topo) {
        for (int v = 0; v < V; ++v) {
            if (graph[u][v] != 0 && u != v) {
                ve[v] = std::max(ve[v], ve[u] + graph[u][v]);
            }
        }
    }

    // 汇点的 ve 即为关键路径长度
    int max_ve = 0;
    for (int v : ve) {
        max_ve = std::max(max_ve, v);
    }

    // 反推：计算 vl（最迟开始时间）
    vl.assign(V, max_ve);
    for (int i = static_cast<int>(topo.size()) - 1; i >= 0; --i) {
        int u = topo[i];
        for (int v = 0; v < V; ++v) {
            if (graph[u][v] != 0 && u != v) {
                vl[u] = std::min(vl[u], vl[v] - graph[u][v]);
            }
        }
    }

    return max_ve;
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_GRAPH_ALGO_H
