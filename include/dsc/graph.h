#ifndef DSC_GRAPH_H
#define DSC_GRAPH_H

#include <vector>
#include <list>
#include <utility>
#include <queue>
#include <stack>

namespace dsc {

// ============================================================
// Graph — 图
// 包含邻接表和邻接矩阵两种表示，各有有向/无向变体
// 顶点用 [0, V-1] 的整数索引表示
// ============================================================

// ============================================================
// AdjListGraph — 邻接表图
// 模板参数 Directed: true=有向图, false=无向图
// ============================================================
template<bool Directed = false>
class AdjListGraph {
public:
    // 边类型：(目标顶点, 权重)
    using Edge = std::pair<int, int>;

    explicit AdjListGraph(int vertices);

    int vertices() const;
    void add_edge(int from, int to, int weight = 1);

    // 获取邻接边
    const std::list<Edge>& neighbors(int v) const;

    // 遍历
    void dfs(int start, void (*visit)(int)) const;
    void bfs(int start, void (*visit)(int)) const;

    // 连通性（仅对无向图有意义）
    bool is_connected() const;

    // 转换为边列表（用于 Kruskal 算法等）
    // 每条边：(权重, from, to)
    std::vector<std::pair<int, std::pair<int, int>>> edges() const;

private:
    int vertices_;
    std::vector<std::list<Edge>> adj_;

    void dfs_util_(int v, std::vector<bool>& visited, void (*visit)(int)) const;
};

// ============================================================
// AdjMatrixGraph — 邻接矩阵图
// 模板参数 Directed: true=有向图, false=无向图
// ============================================================
template<bool Directed = false>
class AdjMatrixGraph {
public:
    explicit AdjMatrixGraph(int vertices);

    int vertices() const;
    void add_edge(int from, int to, int weight = 1);
    int get_weight(int from, int to) const;
    bool has_edge(int from, int to) const;

    // 遍历
    void dfs(int start, void (*visit)(int)) const;
    void bfs(int start, void (*visit)(int)) const;

    // 连通性
    bool is_connected() const;

    // 获取邻接矩阵的只读访问
    const std::vector<std::vector<int>>& matrix() const;

    static constexpr int INF = 0x3f3f3f3f;  // 表示无边

private:
    int vertices_;
    std::vector<std::vector<int>> matrix_;

    void dfs_util_(int v, std::vector<bool>& visited, void (*visit)(int)) const;
};

// ==================== AdjListGraph 实现 ====================

template<bool Directed>
AdjListGraph<Directed>::AdjListGraph(int vertices)
    : vertices_(vertices), adj_(vertices) {}

template<bool Directed>
int AdjListGraph<Directed>::vertices() const {
    return vertices_;
}

template<bool Directed>
void AdjListGraph<Directed>::add_edge(int from, int to, int weight) {
    adj_[from].push_back({to, weight});
    if (!Directed && from != to) {
        adj_[to].push_back({from, weight});
    }
}

template<bool Directed>
const std::list<typename AdjListGraph<Directed>::Edge>&
AdjListGraph<Directed>::neighbors(int v) const {
    return adj_[v];
}

template<bool Directed>
void AdjListGraph<Directed>::dfs(int start, void (*visit)(int)) const {
    std::vector<bool> visited(vertices_, false);
    dfs_util_(start, visited, visit);
}

template<bool Directed>
void AdjListGraph<Directed>::bfs(int start, void (*visit)(int)) const {
    std::vector<bool> visited(vertices_, false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        visit(v);
        for (const auto& edge : adj_[v]) {
            int neighbor = edge.first;
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

template<bool Directed>
bool AdjListGraph<Directed>::is_connected() const {
    std::vector<bool> visited(vertices_, false);
    dfs_util_(0, visited, [](int){});
    for (bool v : visited) {
        if (!v) return false;
    }
    return true;
}

template<bool Directed>
std::vector<std::pair<int, std::pair<int, int>>>
AdjListGraph<Directed>::edges() const {
    std::vector<std::pair<int, std::pair<int, int>>> result;
    for (int u = 0; u < vertices_; ++u) {
        for (const auto& edge : adj_[u]) {
            int v = edge.first;
            int w = edge.second;
            if constexpr (Directed) {
                result.push_back({w, {u, v}});
            } else {
                if (u < v) {  // 无向图每条边只添加一次
                    result.push_back({w, {u, v}});
                }
            }
        }
    }
    return result;
}

template<bool Directed>
void AdjListGraph<Directed>::dfs_util_(
    int v, std::vector<bool>& visited, void (*visit)(int)) const {
    visited[v] = true;
    visit(v);
    for (const auto& edge : adj_[v]) {
        int neighbor = edge.first;
        if (!visited[neighbor]) {
            dfs_util_(neighbor, visited, visit);
        }
    }
}

// ==================== AdjMatrixGraph 实现 ====================

template<bool Directed>
AdjMatrixGraph<Directed>::AdjMatrixGraph(int vertices)
    : vertices_(vertices),
      matrix_(vertices, std::vector<int>(vertices, INF)) {
    for (int i = 0; i < vertices; ++i) {
        matrix_[i][i] = 0;
    }
}

template<bool Directed>
int AdjMatrixGraph<Directed>::vertices() const {
    return vertices_;
}

template<bool Directed>
void AdjMatrixGraph<Directed>::add_edge(int from, int to, int weight) {
    matrix_[from][to] = weight;
    if (!Directed && from != to) {
        matrix_[to][from] = weight;
    }
}

template<bool Directed>
int AdjMatrixGraph<Directed>::get_weight(int from, int to) const {
    return matrix_[from][to];
}

template<bool Directed>
bool AdjMatrixGraph<Directed>::has_edge(int from, int to) const {
    return matrix_[from][to] != INF && from != to;
}

template<bool Directed>
void AdjMatrixGraph<Directed>::dfs(int start, void (*visit)(int)) const {
    std::vector<bool> visited(vertices_, false);
    dfs_util_(start, visited, visit);
}

template<bool Directed>
void AdjMatrixGraph<Directed>::bfs(int start, void (*visit)(int)) const {
    std::vector<bool> visited(vertices_, false);
    std::queue<int> q;
    visited[start] = true;
    q.push(start);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        visit(v);
        for (int i = 0; i < vertices_; ++i) {
            if (matrix_[v][i] != INF && v != i && !visited[i]) {
                visited[i] = true;
                q.push(i);
            }
        }
    }
}

template<bool Directed>
bool AdjMatrixGraph<Directed>::is_connected() const {
    std::vector<bool> visited(vertices_, false);
    dfs_util_(0, visited, [](int){});
    for (bool v : visited) {
        if (!v) return false;
    }
    return true;
}

template<bool Directed>
const std::vector<std::vector<int>>& AdjMatrixGraph<Directed>::matrix() const {
    return matrix_;
}

template<bool Directed>
void AdjMatrixGraph<Directed>::dfs_util_(
    int v, std::vector<bool>& visited, void (*visit)(int)) const {
    visited[v] = true;
    visit(v);
    for (int i = 0; i < vertices_; ++i) {
        if (matrix_[v][i] != INF && v != i && !visited[i]) {
            dfs_util_(i, visited, visit);
        }
    }
}

} // namespace dsc

#endif // DSC_GRAPH_H
