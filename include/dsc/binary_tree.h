#ifndef DSC_BINARY_TREE_H
#define DSC_BINARY_TREE_H

#include <cstddef>
#include <stack>
#include <queue>
#include <utility>

namespace dsc {

// ============================================================
// BinaryTree<T> — 二叉树
// 支持递归/非递归前中后序遍历，以及层序遍历
// 使用函数指针作为访问器：void (*visit)(const T&)
// ============================================================
template<typename T>
class BinaryTree {
private:
    struct Node {
        T     data;
        Node* left;
        Node* right;

        Node(const T& d, Node* l = nullptr, Node* r = nullptr)
            : data(d), left(l), right(r) {}
    };

    Node* root_ = nullptr;

    // 递归遍历辅助
    void pre_order_(Node* node, void (*visit)(const T&));
    void in_order_(Node* node, void (*visit)(const T&));
    void post_order_(Node* node, void (*visit)(const T&));

    // 工具
    int  height_(Node* node) const;
    int  size_(Node* node) const;
    Node* copy_(Node* node);
    bool  is_equal_(Node* a, Node* b) const;
    void  clear_(Node* node);

public:
    // 构造 / 析构
    BinaryTree() = default;
    BinaryTree(const BinaryTree& other);
    BinaryTree(BinaryTree&& other) noexcept;
    ~BinaryTree();

    // 赋值
    BinaryTree& operator=(const BinaryTree& other);
    BinaryTree& operator=(BinaryTree&& other) noexcept;

    // 获取根节点数据（要求非空）
    T&       root();
    const T& root() const;

    // 手动设置根节点
    void set_root(const T& value);

    // 在指定值的节点下插入左右子节点（使用简单的先序遍历查找）
    bool insert_left(const T& parent_value, const T& value);
    bool insert_right(const T& parent_value, const T& value);

    // 遍历（函数指针访问器）
    void pre_order_rec(void (*visit)(const T&));
    void in_order_rec(void (*visit)(const T&));
    void post_order_rec(void (*visit)(const T&));
    void pre_order_nor(void (*visit)(const T&));
    void in_order_nor(void (*visit)(const T&));
    void post_order_nor(void (*visit)(const T&));
    void level_order(void (*visit)(const T&));

    // 属性
    int  height() const;
    int  size() const;
    bool empty() const noexcept;

    // 比较
    bool operator==(const BinaryTree& other) const;
    bool operator!=(const BinaryTree& other) const;

    // 清空
    void clear();
    void swap(BinaryTree& other) noexcept;
};

// ==================== 构造 / 析构 ====================

template<typename T>
BinaryTree<T>::BinaryTree(const BinaryTree& other) {
    if (other.root_) {
        root_ = copy_(other.root_);
    }
}

template<typename T>
BinaryTree<T>::BinaryTree(BinaryTree&& other) noexcept
    : root_(other.root_) {
    other.root_ = nullptr;
}

template<typename T>
BinaryTree<T>::~BinaryTree() {
    clear();
}

// ==================== 赋值 ====================

template<typename T>
BinaryTree<T>& BinaryTree<T>::operator=(const BinaryTree& other) {
    if (this != &other) {
        BinaryTree temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
BinaryTree<T>& BinaryTree<T>::operator=(BinaryTree&& other) noexcept {
    if (this != &other) {
        clear();
        root_ = other.root_;
        other.root_ = nullptr;
    }
    return *this;
}

// ==================== 根节点访问 ====================

template<typename T>
T& BinaryTree<T>::root() {
    return root_->data;
}

template<typename T>
const T& BinaryTree<T>::root() const {
    return root_->data;
}

template<typename T>
void BinaryTree<T>::set_root(const T& value) {
    if (root_) {
        root_->data = value;
    } else {
        root_ = new Node(value);
    }
}

// ==================== 插入（简单先序查找） ====================

template<typename T>
bool BinaryTree<T>::insert_left(const T& parent_value, const T& value) {
    // 层序遍历查找值为 parent_value 的节点
    if (!root_) return false;
    std::queue<Node*> q;
    q.push(root_);
    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();
        if (cur->data == parent_value) {
            if (!cur->left) {
                cur->left = new Node(value);
                return true;
            }
            return false;  // 左子节点已存在
        }
        if (cur->left)  q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
    return false;
}

template<typename T>
bool BinaryTree<T>::insert_right(const T& parent_value, const T& value) {
    if (!root_) return false;
    std::queue<Node*> q;
    q.push(root_);
    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();
        if (cur->data == parent_value) {
            if (!cur->right) {
                cur->right = new Node(value);
                return true;
            }
            return false;  // 右子节点已存在
        }
        if (cur->left)  q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
    return false;
}

// ==================== 递归遍历 ====================

template<typename T>
void BinaryTree<T>::pre_order_rec(void (*visit)(const T&)) {
    pre_order_(root_, visit);
}

template<typename T>
void BinaryTree<T>::in_order_rec(void (*visit)(const T&)) {
    in_order_(root_, visit);
}

template<typename T>
void BinaryTree<T>::post_order_rec(void (*visit)(const T&)) {
    post_order_(root_, visit);
}

// ==================== 非递归遍历 ====================

template<typename T>
void BinaryTree<T>::pre_order_nor(void (*visit)(const T&)) {
    if (!root_) return;
    std::stack<Node*> stk;
    stk.push(root_);
    while (!stk.empty()) {
        Node* cur = stk.top();
        stk.pop();
        visit(cur->data);
        if (cur->right) stk.push(cur->right);
        if (cur->left)  stk.push(cur->left);
    }
}

template<typename T>
void BinaryTree<T>::in_order_nor(void (*visit)(const T&)) {
    std::stack<Node*> stk;
    Node* cur = root_;
    while (cur || !stk.empty()) {
        while (cur) {
            stk.push(cur);
            cur = cur->left;
        }
        cur = stk.top();
        stk.pop();
        visit(cur->data);
        cur = cur->right;
    }
}

template<typename T>
void BinaryTree<T>::post_order_nor(void (*visit)(const T&)) {
    if (!root_) return;
    std::stack<Node*> stk;
    Node* cur = root_;
    Node* last_visited = nullptr;
    while (cur || !stk.empty()) {
        while (cur) {
            stk.push(cur);
            cur = cur->left;
        }
        Node* top = stk.top();
        if (top->right && top->right != last_visited) {
            cur = top->right;
        } else {
            visit(top->data);
            last_visited = top;
            stk.pop();
        }
    }
}

// ==================== 层序遍历 ====================

template<typename T>
void BinaryTree<T>::level_order(void (*visit)(const T&)) {
    if (!root_) return;
    std::queue<Node*> q;
    q.push(root_);
    while (!q.empty()) {
        Node* cur = q.front();
        q.pop();
        visit(cur->data);
        if (cur->left)  q.push(cur->left);
        if (cur->right) q.push(cur->right);
    }
}

// ==================== 属性 ====================

template<typename T>
int BinaryTree<T>::height() const {
    return height_(root_);
}

template<typename T>
int BinaryTree<T>::size() const {
    return size_(root_);
}

template<typename T>
bool BinaryTree<T>::empty() const noexcept {
    return root_ == nullptr;
}

// ==================== 比较 ====================

template<typename T>
bool BinaryTree<T>::operator==(const BinaryTree& other) const {
    return is_equal_(root_, other.root_);
}

template<typename T>
bool BinaryTree<T>::operator!=(const BinaryTree& other) const {
    return !(*this == other);
}

// ==================== 清空 ====================

template<typename T>
void BinaryTree<T>::clear() {
    clear_(root_);
    root_ = nullptr;
}

template<typename T>
void BinaryTree<T>::swap(BinaryTree& other) noexcept {
    using std::swap;
    swap(root_, other.root_);
}

// ==================== 私有辅助 ====================

template<typename T>
void BinaryTree<T>::pre_order_(Node* node, void (*visit)(const T&)) {
    if (!node) return;
    visit(node->data);
    pre_order_(node->left, visit);
    pre_order_(node->right, visit);
}

template<typename T>
void BinaryTree<T>::in_order_(Node* node, void (*visit)(const T&)) {
    if (!node) return;
    in_order_(node->left, visit);
    visit(node->data);
    in_order_(node->right, visit);
}

template<typename T>
void BinaryTree<T>::post_order_(Node* node, void (*visit)(const T&)) {
    if (!node) return;
    post_order_(node->left, visit);
    post_order_(node->right, visit);
    visit(node->data);
}

template<typename T>
int BinaryTree<T>::height_(Node* node) const {
    if (!node) return 0;
    int left_h  = height_(node->left);
    int right_h = height_(node->right);
    return 1 + (left_h > right_h ? left_h : right_h);
}

template<typename T>
int BinaryTree<T>::size_(Node* node) const {
    if (!node) return 0;
    return 1 + size_(node->left) + size_(node->right);
}

template<typename T>
typename BinaryTree<T>::Node* BinaryTree<T>::copy_(Node* node) {
    if (!node) return nullptr;
    Node* new_node = new Node(node->data);
    new_node->left  = copy_(node->left);
    new_node->right = copy_(node->right);
    return new_node;
}

template<typename T>
bool BinaryTree<T>::is_equal_(Node* a, Node* b) const {
    if (!a && !b) return true;
    if (!a || !b) return false;
    return a->data == b->data &&
           is_equal_(a->left, b->left) &&
           is_equal_(a->right, b->right);
}

template<typename T>
void BinaryTree<T>::clear_(Node* node) {
    if (!node) return;
    clear_(node->left);
    clear_(node->right);
    delete node;
}

} // namespace dsc

#endif // DSC_BINARY_TREE_H
