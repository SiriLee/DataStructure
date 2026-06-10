#ifndef DSC_TREE_H
#define DSC_TREE_H

#include <vector>
#include <utility>
#include <string>
#include <queue>
#include <stack>
#include <stdexcept>

namespace dsc {

// ============================================================
// Tree<T> — 一般树
// 每个节点可以有任意数量的子节点
// 支持多种构造方式：先序+后序、双亲表示法、括号表达式
// ============================================================
template<typename T>
class Tree {
private:
    struct Node {
        T                   data;
        std::vector<Node*>  children;

        Node(const T& d) : data(d) {}
    };

    Node* root_ = nullptr;

    // 递归遍历辅助
    void pre_order_(Node* node, void (*visit)(const T&));
    void post_order_(Node* node, void (*visit)(const T&));

    // 构建辅助
    Node* build_from_pre_post_(const std::vector<T>& preorder,
                               const std::vector<T>& postorder,
                               int& pre_idx, int& post_idx);
    Node* build_from_parents_(const std::vector<std::pair<T, int>>& nodes, int degree);
    Node* build_from_expression_(const std::string& expr, std::size_t& pos);
    void  clear_(Node* node);
    Node* copy_(Node* node);

public:
    // 构造 / 析构
    Tree() = default;

    // 由先序和后序遍历序列构造（要求各节点值唯一）
    Tree(const std::vector<T>& preorder, const std::vector<T>& postorder);

    // 由双亲表示法构造：vector<pair<节点值, 父节点索引>>，-1 表示根
    Tree(const std::vector<std::pair<T, int>>& parents);

    // 由括号表达式构造，如 "A(B,C(D,E))"
    explicit Tree(const std::string& expression);

    Tree(const Tree& other);
    Tree(Tree&& other) noexcept;
    ~Tree();

    // 赋值
    Tree& operator=(const Tree& other);
    Tree& operator=(Tree&& other) noexcept;

    // 遍历
    void pre_order_rec(void (*visit)(const T&));
    void post_order_rec(void (*visit)(const T&));
    void pre_order_nor(void (*visit)(const T&));
    void post_order_nor(void (*visit)(const T&));
    void level_order(void (*visit)(const T&));

    // 属性
    bool empty() const noexcept;
    void clear();
    void swap(Tree& other) noexcept;
};

// ==================== 构造 / 析构 ====================

template<typename T>
Tree<T>::Tree(const std::vector<T>& preorder, const std::vector<T>& postorder) {
    int pre_idx  = 0;
    int post_idx = 0;
    root_ = build_from_pre_post_(preorder, postorder, pre_idx, post_idx);
}

template<typename T>
Tree<T>::Tree(const std::vector<std::pair<T, int>>& parents) {
    if (parents.empty()) return;

    std::vector<Node*> nodes;
    for (const auto& p : parents) {
        nodes.push_back(new Node(p.first));
    }

    for (std::size_t i = 0; i < parents.size(); ++i) {
        int parent_idx = parents[i].second;
        if (parent_idx == -1) {
            root_ = nodes[i];
        } else if (parent_idx >= 0 && static_cast<std::size_t>(parent_idx) < nodes.size()) {
            nodes[parent_idx]->children.push_back(nodes[i]);
        }
    }
}

template<typename T>
Tree<T>::Tree(const std::string& expression) {
    std::size_t pos = 0;
    root_ = build_from_expression_(expression, pos);
}

template<typename T>
Tree<T>::Tree(const Tree& other) {
    if (other.root_) {
        root_ = copy_(other.root_);
    }
}

template<typename T>
Tree<T>::Tree(Tree&& other) noexcept
    : root_(other.root_) {
    other.root_ = nullptr;
}

template<typename T>
Tree<T>::~Tree() {
    clear();
}

// ==================== 赋值 ====================

template<typename T>
Tree<T>& Tree<T>::operator=(const Tree& other) {
    if (this != &other) {
        Tree temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
Tree<T>& Tree<T>::operator=(Tree&& other) noexcept {
    if (this != &other) {
        clear();
        root_ = other.root_;
        other.root_ = nullptr;
    }
    return *this;
}

// ==================== 递归遍历 ====================

template<typename T>
void Tree<T>::pre_order_rec(void (*visit)(const T&)) {
    pre_order_(root_, visit);
}

template<typename T>
void Tree<T>::post_order_rec(void (*visit)(const T&)) {
    post_order_(root_, visit);
}

// ==================== 非递归遍历 ====================

template<typename T>
void Tree<T>::pre_order_nor(void (*visit)(const T&)) {
    if (!root_) return;
    std::stack<Node*> stk;
    stk.push(root_);
    while (!stk.empty()) {
        Node* cur = stk.top();
        stk.pop();
        visit(cur->data);
        // 子节点逆序入栈以保证正确顺序
        for (int i = static_cast<int>(cur->children.size()) - 1; i >= 0; --i) {
            stk.push(cur->children[i]);
        }
    }
}

template<typename T>
void Tree<T>::post_order_nor(void (*visit)(const T&)) {
    if (!root_) return;
    // 使用双栈法
    std::stack<Node*> stk1, stk2;
    stk1.push(root_);
    while (!stk1.empty()) {
        Node* cur = stk1.top();
        stk1.pop();
        stk2.push(cur);
        for (Node* child : cur->children) {
            stk1.push(child);
        }
    }
    while (!stk2.empty()) {
        visit(stk2.top()->data);
        stk2.pop();
    }
}

// ==================== 层序遍历 ====================

template<typename T>
void Tree<T>::level_order(void (*visit)(const T&)) {
    if (!root_) return;
    std::queue<Node*> q;
    q.push(root_);
    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();
        visit(cur->data);
        for (Node* child : cur->children) {
            q.push(child);
        }
    }
}

// ==================== 属性 ====================

template<typename T>
bool Tree<T>::empty() const noexcept {
    return root_ == nullptr;
}

template<typename T>
void Tree<T>::clear() {
    clear_(root_);
    root_ = nullptr;
}

template<typename T>
void Tree<T>::swap(Tree& other) noexcept {
    using std::swap;
    swap(root_, other.root_);
}

// ==================== 私有辅助 — 遍历 ====================

template<typename T>
void Tree<T>::pre_order_(Node* node, void (*visit)(const T&)) {
    if (!node) return;
    visit(node->data);
    for (Node* child : node->children) {
        pre_order_(child, visit);
    }
}

template<typename T>
void Tree<T>::post_order_(Node* node, void (*visit)(const T&)) {
    if (!node) return;
    for (Node* child : node->children) {
        post_order_(child, visit);
    }
    visit(node->data);
}

// ==================== 私有辅助 — 构建 ====================

template<typename T>
typename Tree<T>::Node* Tree<T>::build_from_pre_post_(
    const std::vector<T>& preorder,
    const std::vector<T>& postorder,
    int& pre_idx, int& post_idx) {

    if (pre_idx >= static_cast<int>(preorder.size())) return nullptr;

    Node* node = new Node(preorder[pre_idx++]);

    // 当前节点的子树范围在 postorder 中由 post_idx 界定
    // 当 node->data != postorder[post_idx]，说明还有子节点未处理
    while (post_idx < static_cast<int>(postorder.size()) &&
           (node->children.empty() ||
            node->children.back()->data != postorder[post_idx])) {
        Node* child = build_from_pre_post_(preorder, postorder, pre_idx, post_idx);
        if (child) node->children.push_back(child);
    }
    ++post_idx;  // 消费 postorder 中当前节点
    return node;
}

template<typename T>
typename Tree<T>::Node* Tree<T>::build_from_expression_(
    const std::string& expr, std::size_t& pos) {

    // 跳过空格
    while (pos < expr.size() &&
           (expr[pos] == ' ' || expr[pos] == '\t')) ++pos;
    if (pos >= expr.size()) return nullptr;

    // 读取节点名（假设为单字符，模板化后使用第一个非括号字符）
    char ch = expr[pos++];
    Node* node = new Node(T(ch));

    // 跳过空格
    while (pos < expr.size() &&
           (expr[pos] == ' ' || expr[pos] == '\t')) ++pos;

    // 如果有子节点
    if (pos < expr.size() && expr[pos] == '(') {
        ++pos;  // 跳过 '('
        while (pos < expr.size() && expr[pos] != ')') {
            if (expr[pos] == ',') { ++pos; continue; }
            Node* child = build_from_expression_(expr, pos);
            if (child) node->children.push_back(child);
        }
        ++pos;  // 跳过 ')'
    }
    return node;
}

// ==================== 私有辅助 — 清理/复制 ====================

template<typename T>
void Tree<T>::clear_(Node* node) {
    if (!node) return;
    for (Node* child : node->children) {
        clear_(child);
    }
    delete node;
}

template<typename T>
typename Tree<T>::Node* Tree<T>::copy_(Node* node) {
    if (!node) return nullptr;
    Node* new_node = new Node(node->data);
    for (Node* child : node->children) {
        new_node->children.push_back(copy_(child));
    }
    return new_node;
}

} // namespace dsc

#endif // DSC_TREE_H
