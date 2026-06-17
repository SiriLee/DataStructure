# DataStructure

简洁、易读、易用的 C++ 数据结构与算法库（header-only）。

## 特性

- **Header-only**：只需将 `include/dsc/` 复制到项目中即可使用，无需编译或安装
- **命名空间 `dsc`**：所有数据结构位于 `dsc::`，算法位于 `dsc::algorithm::`
- **清晰命名**：成员变量统一使用尾下划线风格（如 `size_`、`data_`）
- **自包含**：每个头文件独立完整，可直接阅读单个文件理解实现
- **无迭代器**：全部使用索引和指针访问，简单直接
- **C++17 及以上**：使用基本的 C++17 特性

## 快速开始

```cpp
#include <dsc/dsc.h>
#include <iostream>

int main() {
    // 使用 Vector
    dsc::Vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);

    for (std::size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    // 使用排序算法
    dsc::Vector<int> vec2 = {3, 1, 4, 1, 5};
    dsc::algorithm::QuickSort(vec2, 0, vec2.size() - 1);

    return 0;
}
```

## 目录结构

```
DataStructure/
├── README.md
├── CMakeLists.txt              # 可选的 CMake 构建（仅用于测试）
├── .gitignore
├── include/dsc/
│   ├── dsc.h                   # 统一包含头文件
│   ├── array.h                 # Array<T, N>      — 固定大小数组
│   ├── vector.h                # Vector<T>        — 动态数组
│   ├── list.h                  # List<T>          — 双向链表
│   ├── stack.h                 # Stack<T>         — 栈
│   ├── queue.h                 # Queue<T>         — 队列
│   ├── string.h                # String           — 字符串
│   ├── set.h                   # Set<T>           — 二叉搜索树集合
│   ├── avl_tree.h              # AVLTree          — AVL树
│   ├── binary_tree.h           # BinaryTree<T>    — 二叉树
│   ├── rb_tree.h               # RBTree           — 红黑树
│   ├── tree.h                  # Tree<T>          — 一般树
│   ├── trie.h                  # Trie             — 字典树（前缀树）
│   ├── disjoint_sets.h         # CompressDS / RankDS — 并查集（路径压缩 / 按秩合并）
│   ├── graph.h                 # Graph            — 图（邻接表/矩阵，有向/无向）
│   ├── loser_tree.h            # LoserTree<T>     — 败者树
│   └── algorithm/
│       ├── algorithm.h         # 统一算法包含
│       ├── sort.h              # 排序算法
│       ├── external_sort.h     # 外部排序
│       ├── kmp.h               # KMP 字符串匹配
│       ├── huffman.h           # 哈夫曼编码
│       ├── expression.h        # 表达式求值
│       ├── rolling_hash.h      # 滚动哈希
│       ├── shortest_path.h     # 最短路径
│       ├── mst.h               # 最小生成树
│       ├── graph_algo.h        # 拓扑排序、关键路径
│       └── euler.h             # 欧拉回路/路径
├── tests/                      # 测试文件
└── examples/                   # 使用示例
```

## 数据结构概览

| 类 | 描述 | 命名空间 |
|---|---|---|
| `Array<T, N>` | 固定大小数组，编译期确定大小 | `dsc::` |
| `Vector<T>` | 动态数组，自动扩容 | `dsc::` |
| `List<T>` | 双向链表 | `dsc::` |
| `Stack<T>` | 栈（LIFO） | `dsc::` |
| `Queue<T>` | 队列（FIFO），环形缓冲区实现 | `dsc::` |
| `String` | 字符串类 | `dsc::` |
| `Set<T>` | 基于二叉搜索树的集合 | `dsc::` |
| `AVLTree` | AVL树，自平衡二叉搜索树 | `dsc::` |
| `BinaryTree<T>` | 二叉树，支持递归/非递归/层序遍历 | `dsc::` |
| `RBTree` | 红黑树，支持插入和查找 | `dsc::` |
| `Tree<T>` | 一般树，支持多种构造方式 | `dsc::` |
| `Trie` | 字典树（前缀树），支持小写字母 a-z | `dsc::` |
| `CompressDS` | 并查集（路径压缩） | `dsc::` |
| `RankDS` | 并查集（按秩合并） | `dsc::` |
| `AdjListGraph` | 邻接表图 | `dsc::` |
| `AdjMatrixGraph` | 邻接矩阵图 | `dsc::` |
| `LoserTree<T>` | 败者树（最小胜者树），O(k) 建树 O(log k) 重赛 | `dsc::` |

## 算法概览

| 算法 | 函数 | 命名空间 |
|---|---|---|
| 冒泡排序 | `BubbleSort` | `dsc::algorithm::` |
| 插入排序 | `InsertionSort` | `dsc::algorithm::` |
| 希尔排序 | `ShellSort` | `dsc::algorithm::` |
| 选择排序 | `SelectionSort` | `dsc::algorithm::` |
| 堆排序 | `HeapSort` | `dsc::algorithm::` |
| 快速排序（递归） | `QuickSort` | `dsc::algorithm::` |
| 快速排序（迭代） | `QuickSortIterative` | `dsc::algorithm::` |
| 归并排序（迭代） | `MergeSort` | `dsc::algorithm::` |
| 归并排序（递归） | `MergeSortRecursive` | `dsc::algorithm::` |
| 基数排序（LSD） | `RadixSortLSD` | `dsc::algorithm::` |
| 置换-选择排序 | `ReplacementSelection` | `dsc::algorithm::` |
| k-路归并 | `KWayMerge` | `dsc::algorithm::` |
| 外部排序 | `ExternalSort` | `dsc::algorithm::` |
| KMP 字符串匹配 | `KmpSearch` / `KmpSearchFixed` | `dsc::algorithm::` |
| 哈夫曼编码 | `HuffmanTree` 类 | `dsc::algorithm::` |
| 表达式求值 | `PostfixEval` / `PrefixEval` / `InfixEval` | `dsc::algorithm::` |
| 滚动哈希 | `RollingHash` / `RollingHashPlus` | `dsc::algorithm::` |
| Dijkstra 最短路径 | `Dijkstra` | `dsc::algorithm::` |
| Floyd 最短路径 | `Floyd` | `dsc::algorithm::` |
| Warshall 传递闭包 | `Warshall` | `dsc::algorithm::` |
| Kruskal 最小生成树 | `Kruskal` | `dsc::algorithm::` |
| Prim 最小生成树 | `Prim` | `dsc::algorithm::` |
| 拓扑排序 | `TopologicalSort` | `dsc::algorithm::` |
| 关键路径 | `CriticalPath` | `dsc::algorithm::` |
| 欧拉回路/路径 | `Eulerian` 类 | `dsc::algorithm::` |

## 使用方式

### 方式一：直接复制头文件（推荐）

将 `include/dsc/` 目录复制到你的项目的 include 路径中，然后：

```cpp
#include <dsc/dsc.h>
```

### 方式二：使用 CMake

```bash
# 首次配置（MinGW）
cmake --preset mingw

# 编译
cmake --build build

# 运行测试
ctest --test-dir build
```

## 命名规范

- 类名：大驼峰，如 `BinaryTree`、`AdjListGraph`
- 函数名：大驼峰，如 `PushBack`、`QuickSort`
- 成员变量：小写 + 尾下划线，如 `size_`、`data_`、`head_`
- 私有辅助函数：小写 + 尾下划线，如 `reallocate_()`、`get_node_at_()`

## 许可

此项目仅用于学习和参考。
