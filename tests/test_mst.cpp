#include <dsc/algorithm/mst.h>
#include <iostream>
#include <cassert>

int main() {
    using namespace dsc::algorithm;

    // Kruskal
    {
        std::vector<Edge> edges = {
            {1, {0, 1}},
            {3, {0, 2}},
            {4, {1, 2}},
            {2, {1, 3}},
            {5, {2, 3}},
        };
        auto mst = Kruskal(edges, 4);
        // MST 权重：1 + 2 + 3 = 6
        int total = 0;
        for (const auto& e : mst) total += e.first;
        assert(static_cast<int>(mst.size()) == 3);
        assert(total == 6);
    }

    // Prim
    {
        int INF_ = 0x3f3f3f3f;
        std::vector<std::vector<int>> graph = {
            {0, 2, 0, 6, 0},
            {2, 0, 3, 8, 5},
            {0, 3, 0, 0, 7},
            {6, 8, 0, 0, 9},
            {0, 5, 7, 9, 0},
        };
        // 0 替换为 INF
        for (auto& row : graph)
            for (int& w : row)
                if (w == 0) w = INF_MST;
        for (int i = 0; i < 5; ++i) graph[i][i] = 0;

        int total = Prim(graph);
        assert(total == 16);  // 2+3+5+6 = 16

        // PrimNaive should produce the same result
        int total_naive = PrimNaive(graph);
        assert(total_naive == total);
    }

    std::cout << "test_mst: all tests passed!" << std::endl;
    return 0;
}
