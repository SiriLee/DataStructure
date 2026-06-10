#ifndef DSC_SET_H
#define DSC_SET_H

#include <cstddef>
#include <utility>

namespace dsc {

// ============================================================
// Set<T> — 基于二叉搜索树（BST）的集合
// 元素唯一，支持插入、删除、查找
// ============================================================
template<typename T>
class Set {
private:
    struct Node {
        T     data;
        Node* left;
        Node* right;
        Node* parent;

        Node(const T& val, Node* p = nullptr)
            : data(val), left(nullptr), right(nullptr), parent(p) {}
        Node(T&& val, Node* p = nullptr)
            : data(std::move(val)), left(nullptr), right(nullptr), parent(p) {}
    };

    Node*       root_ = nullptr;
    std::size_t size_ = 0;

    // 私有辅助
    Node* copy_tree_(Node* node, Node* parent);
    void  delete_tree_(Node* node);
    Node* insert_node_(Node* node, Node* parent, const T& value);
    Node* insert_node_(Node* node, Node* parent, T&& value);
    Node* find_node_(Node* node, const T& value) const;
    Node* find_min_(Node* node) const;
    Node* find_max_(Node* node) const;
    void  transplant_(Node* u, Node* v);
    void  remove_node_(Node* z);

public:
    // 构造 / 析构
    Set() = default;
    Set(const Set& other);
    Set(Set&& other) noexcept;
    ~Set();

    // 赋值
    Set& operator=(const Set& other);
    Set& operator=(Set&& other) noexcept;

    // 容量
    bool        empty() const noexcept;
    std::size_t size() const noexcept;

    // 修改
    void clear() noexcept;
    bool insert(const T& value);
    bool insert(T&& value);
    void erase(const T& value);
    void swap(Set& other) noexcept;

    // 查找
    bool        contains(const T& value) const;
    std::size_t count(const T& value) const;

    // 访问最值（要求集合非空）
    const T& min() const;
    const T& max() const;
};

// ==================== 构造 / 析构 ====================

template<typename T>
Set<T>::Set(const Set& other) : root_(nullptr), size_(0) {
    if (other.root_) {
        root_ = copy_tree_(other.root_, nullptr);
        size_ = other.size_;
    }
}

template<typename T>
Set<T>::Set(Set&& other) noexcept
    : root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
    other.size_ = 0;
}

template<typename T>
Set<T>::~Set() {
    clear();
}

// ==================== 赋值 ====================

template<typename T>
Set<T>& Set<T>::operator=(const Set& other) {
    if (this != &other) {
        Set temp(other);
        swap(temp);
    }
    return *this;
}

template<typename T>
Set<T>& Set<T>::operator=(Set&& other) noexcept {
    if (this != &other) {
        clear();
        root_ = other.root_;
        size_ = other.size_;
        other.root_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

// ==================== 容量 ====================

template<typename T>
bool Set<T>::empty() const noexcept {
    return size_ == 0;
}

template<typename T>
std::size_t Set<T>::size() const noexcept {
    return size_;
}

// ==================== 修改 ====================

template<typename T>
void Set<T>::clear() noexcept {
    delete_tree_(root_);
    root_ = nullptr;
    size_ = 0;
}

template<typename T>
bool Set<T>::insert(const T& value) {
    if (contains(value)) return false;
    root_ = insert_node_(root_, nullptr, value);
    ++size_;
    return true;
}

template<typename T>
bool Set<T>::insert(T&& value) {
    if (contains(value)) return false;
    root_ = insert_node_(root_, nullptr, std::move(value));
    ++size_;
    return true;
}

template<typename T>
void Set<T>::erase(const T& value) {
    Node* node = find_node_(root_, value);
    if (node) {
        remove_node_(node);
        --size_;
    }
}

template<typename T>
void Set<T>::swap(Set& other) noexcept {
    using std::swap;
    swap(root_, other.root_);
    swap(size_, other.size_);
}

// ==================== 查找 ====================

template<typename T>
bool Set<T>::contains(const T& value) const {
    return find_node_(root_, value) != nullptr;
}

template<typename T>
std::size_t Set<T>::count(const T& value) const {
    return contains(value) ? 1 : 0;
}

// ==================== 最值访问 ====================

template<typename T>
const T& Set<T>::min() const {
    Node* node = find_min_(root_);
    return node->data;
}

template<typename T>
const T& Set<T>::max() const {
    Node* node = find_max_(root_);
    return node->data;
}

// ==================== 私有辅助 ====================

template<typename T>
typename Set<T>::Node* Set<T>::copy_tree_(Node* node, Node* parent) {
    if (!node) return nullptr;
    Node* new_node = new Node(node->data, parent);
    new_node->left  = copy_tree_(node->left, new_node);
    new_node->right = copy_tree_(node->right, new_node);
    return new_node;
}

template<typename T>
void Set<T>::delete_tree_(Node* node) {
    if (!node) return;
    delete_tree_(node->left);
    delete_tree_(node->right);
    delete node;
}

template<typename T>
typename Set<T>::Node* Set<T>::insert_node_(Node* node, Node* parent, const T& value) {
    if (!node) return new Node(value, parent);
    if (value < node->data)
        node->left = insert_node_(node->left, node, value);
    else if (value > node->data)
        node->right = insert_node_(node->right, node, value);
    return node;
}

template<typename T>
typename Set<T>::Node* Set<T>::insert_node_(Node* node, Node* parent, T&& value) {
    if (!node) return new Node(std::move(value), parent);
    if (value < node->data)
        node->left = insert_node_(node->left, node, std::move(value));
    else if (value > node->data)
        node->right = insert_node_(node->right, node, std::move(value));
    return node;
}

template<typename T>
typename Set<T>::Node* Set<T>::find_node_(Node* node, const T& value) const {
    if (!node || node->data == value) return node;
    if (value < node->data) return find_node_(node->left, value);
    return find_node_(node->right, value);
}

template<typename T>
typename Set<T>::Node* Set<T>::find_min_(Node* node) const {
    while (node && node->left) node = node->left;
    return node;
}

template<typename T>
typename Set<T>::Node* Set<T>::find_max_(Node* node) const {
    while (node && node->right) node = node->right;
    return node;
}

template<typename T>
void Set<T>::transplant_(Node* u, Node* v) {
    if (!u->parent)
        root_ = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v) v->parent = u->parent;
}

template<typename T>
void Set<T>::remove_node_(Node* z) {
    if (!z->left) {
        transplant_(z, z->right);
    } else if (!z->right) {
        transplant_(z, z->left);
    } else {
        Node* y = find_min_(z->right);
        if (y->parent != z) {
            transplant_(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant_(z, y);
        y->left = z->left;
        y->left->parent = y;
    }
    delete z;
}

} // namespace dsc

#endif // DSC_SET_H
