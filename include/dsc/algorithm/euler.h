#ifndef DSC_ALGORITHM_EULER_H
#define DSC_ALGORITHM_EULER_H

#include <vector>
#include <queue>

namespace dsc {
namespace algorithm {

// ============================================================
// Eulerian — 欧拉回路/路径
// 使用邻接矩阵 graph[u][v] = k 表示 k 条边，0 表示无边
// 支持有向图和无向图，支持多重边
// ============================================================
class Eulerian {
public:
    // graph: 邻接矩阵，graph[u][v] = 边的条数
    // directed: true=有向图, false=无向图（要求 graph 对称）
    explicit Eulerian(const std::vector<std::vector<int>>& graph, bool directed);

    // 判断是否存在欧拉回路
    bool HasEulerCircuit() const;

    // 判断是否存在欧拉路径
    bool HasEulerPath() const;

    // 求欧拉回路，返回顶点序列
    // start = -1: 自动选择第一个非零度顶点
    // 不存在回路或 start 为孤立点时返回空
    std::vector<int> EulerCircuit(int start = -1) const;

private:
    std::vector<std::vector<int>> graph_;
    bool directed_;
    int vertex_count_;

    bool is_connected_() const;
    std::vector<int> odd_vertices_() const;
    int degree_(int u) const;
    void hierholzer_(int u, std::vector<std::vector<int>>& temp,
                     std::vector<int>& result) const;
};

// ==================== 实现 ====================

inline Eulerian::Eulerian(const std::vector<std::vector<int>>& graph, bool directed)
    : graph_(graph), directed_(directed), vertex_count_(static_cast<int>(graph.size())) {}

inline bool Eulerian::HasEulerCircuit() const {
    if (vertex_count_ == 0) return false;
    if (!is_connected_()) return false;
    return odd_vertices_().empty();
}

inline bool Eulerian::HasEulerPath() const {
    if (vertex_count_ == 0) return false;
    if (!is_connected_()) return false;
    auto odds = odd_vertices_();
    return odds.size() == 0 || odds.size() == 2;
}

inline std::vector<int> Eulerian::EulerCircuit(int start) const {
    if (!HasEulerCircuit()) return {};

    if (start == -1) {
        for (int i = 0; i < vertex_count_; ++i) {
            if (degree_(i) > 0) {
                start = i;
                break;
            }
        }
        if (start == -1) return {};
    } else if (degree_(start) == 0) {
        return {};
    }

    auto temp = graph_;
    std::vector<int> result;
    hierholzer_(start, temp, result);

    std::vector<int> reversed(result.rbegin(), result.rend());
    return reversed;
}

inline bool Eulerian::is_connected_() const {
    int start = -1;
    for (int i = 0; i < vertex_count_; ++i) {
        if (degree_(i) > 0) {
            start = i;
            break;
        }
    }
    if (start == -1) return false;

    std::vector<bool> visited(vertex_count_, false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v = 0; v < vertex_count_; ++v) {
            if (visited[v]) continue;
            bool edge = (graph_[u][v] > 0) || (graph_[v][u] > 0);
            if (edge) {
                visited[v] = true;
                q.push(v);
            }
        }
    }

    for (int i = 0; i < vertex_count_; ++i) {
        if (degree_(i) > 0 && !visited[i]) return false;
    }
    return true;
}

inline std::vector<int> Eulerian::odd_vertices_() const {
    std::vector<int> odds;
    if (directed_) {
        for (int i = 0; i < vertex_count_; ++i) {
            int out_deg = 0;
            int in_deg = 0;
            for (int j = 0; j < vertex_count_; ++j) {
                out_deg += graph_[i][j];
                in_deg += graph_[j][i];
            }
            if (out_deg != in_deg) {
                odds.push_back(i);
            }
        }
    } else {
        for (int i = 0; i < vertex_count_; ++i) {
            if (degree_(i) % 2 != 0) {
                odds.push_back(i);
            }
        }
    }
    return odds;
}

inline int Eulerian::degree_(int u) const {
    int deg = 0;
    for (int v = 0; v < vertex_count_; ++v) {
        deg += graph_[u][v];
    }
    // self-loop contributes 2 to degree in undirected graphs
    if (!directed_) deg += graph_[u][u];
    return deg;
}

inline void Eulerian::hierholzer_(int u, std::vector<std::vector<int>>& temp,
                                   std::vector<int>& result) const {
    for (int v = 0; v < vertex_count_; ++v) {
        while (temp[u][v] > 0) {
            --temp[u][v];
            if (!directed_ && u != v) --temp[v][u];
            hierholzer_(v, temp, result);
        }
    }
    result.push_back(u);
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_EULER_H
