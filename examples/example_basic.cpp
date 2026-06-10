// ============================================================
// example_basic.cpp — 基本使用示例
//
// 编译（无需 CMake）：
//   g++ -std=c++17 -I ../include example_basic.cpp -o example_basic
// ============================================================

#include <dsc/dsc.h>
#include <iostream>
#include <vector>

int main() {
    std::cout << "=== DataStructure Library Demo ===" << std::endl;

    // ---- Vector ----
    std::cout << "\n--- Vector ---" << std::endl;
    dsc::Vector<int> vec = {3, 1, 4, 1, 5};
    std::cout << "Original: ";
    for (std::size_t i = 0; i < vec.size(); ++i)
        std::cout << vec[i] << " ";
    std::cout << std::endl;

    vec.push_back(9);
    vec.pop_back();
    std::cout << "Size: " << vec.size() << std::endl;

    // ---- List ----
    std::cout << "\n--- List ---" << std::endl;
    dsc::List<std::string> list;
    list.push_back("hello");
    list.push_back("world");
    list.push_front(">>>");
    std::cout << "List: " << list.front() << " ... " << list.back()
              << " (size=" << list.size() << ")" << std::endl;

    // ---- Stack ----
    std::cout << "\n--- Stack ---" << std::endl;
    dsc::Stack<int> stk;
    stk.push(10);
    stk.push(20);
    stk.push(30);
    std::cout << "Stack top: " << stk.top() << ", size: " << stk.size() << std::endl;
    stk.pop();
    std::cout << "After pop, top: " << stk.top() << std::endl;

    // ---- Queue ----
    std::cout << "\n--- Queue ---" << std::endl;
    dsc::Queue<int> q;
    q.push(1);
    q.push(2);
    q.push(3);
    std::cout << "Queue front: " << q.front() << ", back: " << q.back() << std::endl;
    q.pop();
    std::cout << "After pop, front: " << q.front() << std::endl;

    // ---- String ----
    std::cout << "\n--- String ---" << std::endl;
    dsc::String s = "Hello";
    s += " DataStructure!";
    std::cout << s.c_str() << " (length=" << s.length() << ")" << std::endl;
    std::cout << "Find 'Data': " << s.find("Data") << std::endl;

    // ---- Set ----
    std::cout << "\n--- Set ---" << std::endl;
    dsc::Set<int> set;
    set.insert(5);
    set.insert(3);
    set.insert(8);
    set.insert(1);
    std::cout << "Set size: " << set.size()
              << ", min: " << set.min() << ", max: " << set.max() << std::endl;
    std::cout << "Contains 3? " << (set.contains(3) ? "yes" : "no") << std::endl;

    // ---- Sorting ----
    std::cout << "\n--- Sorting ---" << std::endl;
    std::vector<int> to_sort = {64, 34, 25, 12, 22, 11, 90};
    dsc::algorithm::QuickSort(to_sort, 0, static_cast<int>(to_sort.size()) - 1);
    std::cout << "Sorted: ";
    for (int x : to_sort) std::cout << x << " ";
    std::cout << std::endl;

    // ---- Graph ----
    std::cout << "\n--- Graph (AdjList, undirected) ---" << std::endl;
    dsc::AdjListGraph<false> g(5);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(3, 4);
    std::cout << "Vertices: " << g.vertices() << std::endl;
    std::cout << "Connected? " << (g.is_connected() ? "yes" : "no") << std::endl;

    // ---- KMP ----
    std::cout << "\n--- KMP ---" << std::endl;
    int pos = dsc::algorithm::KmpSearch("hello world", "world");
    std::cout << "'world' found at position: " << pos << std::endl;

    // ---- BinaryTree ----
    std::cout << "\n--- BinaryTree ---" << std::endl;
    dsc::BinaryTree<int> bt;
    bt.set_root(1);
    bt.insert_left(1, 2);
    bt.insert_right(1, 3);
    std::cout << "Root: " << bt.root() << ", size: " << bt.size()
              << ", height: " << bt.height() << std::endl;

    std::cout << "\n=== All examples completed ===" << std::endl;
    return 0;
}
