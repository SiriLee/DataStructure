#ifndef DSC_AVL_TREE_H
#define DSC_AVL_TREE_H

#include <algorithm>

namespace dsc {

// ============================================================
// AVLTree<T> — AVL树
// 实现：搜索、插入、删除
// ============================================================

class AVLTree {
private:
    struct Node {
        int data;
        Node* left;
        Node* right;
        int height;

        Node(int d) : data(d), left(nullptr), right(nullptr), height(1) {}
    };
    Node* root_ = nullptr;
public:
    AVLTree() = default;
    ~AVLTree() { clear(root_); }

    void insert(int value);
    void remove(int value);
    bool search(int value) const;
private:
    // AVL树辅助函数
    int height(Node* node) const;
    int balance_factor(Node* node) const;
    void update_height(Node* node);

    // 旋转和调整
    Node* rotate_right(Node* y);
    Node* rotate_left(Node* x);
    Node* balance(Node* node);
    
    // 递归实现
    Node* insert(Node* node, int value);
    Node* remove(Node* node, int value);
    bool search(Node* node, int value) const;

    // 释放节点
    void clear(Node* node);
};

void AVLTree::insert(int value) {
    root_ = insert(root_, value);
}

void AVLTree::remove(int value) {
    root_ = remove(root_, value);
}

bool AVLTree::search(int value) const {
    return search(root_, value);
}

int AVLTree::height(Node* node) const {
    return node ? node->height : 0;
}

int AVLTree::balance_factor(Node* node) const {
    return height(node->left) - height(node->right);
}

void AVLTree::update_height(Node* node) {
    if (node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }
}

AVLTree::Node* AVLTree::rotate_right(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // 旋转
    x->right = y;
    y->left = T2;

    // 更新高度
    update_height(y);
    update_height(x);

    return x; // 新根节点
}

AVLTree::Node* AVLTree::rotate_left(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // 旋转
    y->left = x;
    x->right = T2;

    // 更新高度
    update_height(x);
    update_height(y);

    return y; // 新根节点
}

AVLTree::Node* AVLTree::balance(Node* node) {
    update_height(node);
    int bf = balance_factor(node);

    // 左重
    if (bf > 1) {
        if (balance_factor(node->left) < 0) {
            node->left = rotate_left(node->left);
        }
        return rotate_right(node);
    }
    // 右重
    if (bf < -1) {
        if (balance_factor(node->right) > 0) {
            node->right = rotate_right(node->right);
        }
        return rotate_left(node);
    }
    return node; // 已平衡
}

AVLTree::Node* AVLTree::insert(Node* node, int value) {
    if (!node) return new Node(value);
    if (value < node->data) {
        node->left = insert(node->left, value);
    } else if (value > node->data) {
        node->right = insert(node->right, value);
    } else {
        return node; // 不允许重复
    }
    return balance(node);
}

AVLTree::Node* AVLTree::remove(Node* node, int value) {
    if (!node) return node;
    if (value < node->data) {
        node->left = remove(node->left, value);
    } else if (value > node->data) {
        node->right = remove(node->right, value);
    } else {
        // 找到节点
        if (!node->left || !node->right) {
            Node* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        } else {
            // 找到右子树的最小节点
            Node* temp = node->right;
            while (temp->left) temp = temp->left;
            node->data = temp->data; // 替换数据
            node->right = remove(node->right, temp->data); // 删除右子树的最小节点
        }
    }
    return balance(node);
}

bool AVLTree::search(Node* node, int value) const {
    if (!node) return false;
    if (value < node->data) {
        return search(node->left, value);
    } else if (value > node->data) {
        return search(node->right, value);
    } else {
        return true; // 找到
    }
}

void AVLTree::clear(Node* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

} // namespace dsc

#endif // DSC_AVL_TREE_H