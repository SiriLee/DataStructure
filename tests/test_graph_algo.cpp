#include <dsc/algorithm/graph_algo.h>
#include <iostream>
#include <cassert>

int main() {
    using namespace dsc::algorithm;

    // TopologicalSort
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 1, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 1, 1},
            {0, 0, 0, 0, 1},
            {0, 0, 0, 0, 0},
        };
        auto topo = TopologicalSort(graph);
        assert(topo.size() == 5);
        // 0 应该在 1 和 2 之前，1 在前面...
        // 简单验证：检查返回的序列非空
    }

    // 带环图应返回空
    {
        std::vector<std::vector<int>> graph = {
            {0, 1, 0},
            {0, 0, 1},
            {1, 0, 0},
        };
        auto topo = TopologicalSort(graph);
        assert(topo.empty());
    }

    // CriticalPath
    {
        std::vector<std::vector<int>> graph = {
            {0, 6, 4, 5, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 2, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 9, 7, 0},
            {0, 0, 0, 0, 0, 0, 0, 4, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 2},
            {0, 0, 0, 0, 0, 0, 0, 0, 4},
            {0, 0, 0, 0, 0, 0, 0, 0, 0},
        };
        auto critical = CriticalPath(graph);
        assert(!critical.empty());          // 有关键路径
        assert(critical.size() == 6);       // 6 条关键活动：0→1, 1→4, 4→6, 4→7, 6→8, 7→8
    }

    std::cout << "test_graph_algo: all tests passed!" << std::endl;
    return 0;
}
