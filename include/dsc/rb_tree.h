#ifndef DSC_RB_TREE_H
#define DSC_RB_TREE_H

namespace dsc {

// ============================================================
// RBTree — 红黑树
// 实现：搜索、插入
// ============================================================

class RBTree {
private:
    enum Color { RED, BLACK };

    struct Node {
        int data;
        Node* left;
        Node* right;
        Node* parent;
        Color color;

        Node(int d)
            : data(d), left(nullptr), right(nullptr), parent(nullptr), color(RED) {}
    };

    Node* root_ = nullptr;

public:
    RBTree() = default;
    ~RBTree() { clear(root_); }

    void insert(int value);
    bool search(int value) const;

private:
    // 旋转（含父指针维护）
    Node* rotate_left(Node* x);
    Node* rotate_right(Node* y);

    // 插入修正：修复连续红色违规
    void insert_fixup(Node* node);

    // 辅助
    void clear(Node* node);
};

// ==================== 旋转 ====================

RBTree::Node* RBTree::rotate_left(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // 旋转
    y->left = x;
    x->right = T2;

    // 父指针
    y->parent = x->parent;
    x->parent = y;
    if (T2) T2->parent = x;

    // 祖父指向新根
    if (!y->parent) {
        root_ = y;
    } else if (y->parent->left == x) {
        y->parent->left = y;
    } else {
        y->parent->right = y;
    }

    return y;
}

RBTree::Node* RBTree::rotate_right(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // 旋转
    x->right = y;
    y->left = T2;

    // 父指针
    x->parent = y->parent;
    y->parent = x;
    if (T2) T2->parent = y;

    // 祖父指向新根
    if (!x->parent) {
        root_ = x;
    } else if (x->parent->left == y) {
        x->parent->left = x;
    } else {
        x->parent->right = x;
    }

    return x;
}

// ==================== 插入 ====================

void RBTree::insert(int value) {
    Node* node = new Node(value);
    Node* parent = nullptr;
    Node* curr = root_;

    // BST 插入
    while (curr) {
        parent = curr;
        if (value < curr->data) {
            curr = curr->left;
        } else if (value > curr->data) {
            curr = curr->right;
        } else {
            delete node; // 不允许重复
            return;
        }
    }

    node->parent = parent;
    if (!parent) {
        root_ = node;
    } else if (value < parent->data) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    insert_fixup(node);
}

void RBTree::insert_fixup(Node* node) {
    // 新节点为红色，若父节点为红则违规
    while (node != root_ && node->parent->color == RED) {
        Node* gp = node->parent->parent; // 祖父一定存在（父为红，祖父必为黑）

        if (node->parent == gp->left) {
            Node* uncle = gp->right;
            if (uncle && uncle->color == RED) {
                // 情况1：叔节点为红 → 重新染色
                node->parent->color = BLACK;
                uncle->color = BLACK;
                gp->color = RED;
                node = gp;
            } else {
                if (node == node->parent->right) {
                    // 情况2：内侧孙子 → 旋转为外侧
                    node = node->parent;
                    rotate_left(node);
                }
                // 情况3：外侧孙子 → 旋转 + 染色
                node->parent->color = BLACK;
                gp->color = RED;
                rotate_right(gp);
            }
        } else {
            // 对称：父为右子
            Node* uncle = gp->left;
            if (uncle && uncle->color == RED) {
                node->parent->color = BLACK;
                uncle->color = BLACK;
                gp->color = RED;
                node = gp;
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rotate_right(node);
                }
                node->parent->color = BLACK;
                gp->color = RED;
                rotate_left(gp);
            }
        }
    }
    root_->color = BLACK;
}

// ==================== 查找 ====================

bool RBTree::search(int value) const {
    Node* curr = root_;
    while (curr) {
        if (value < curr->data) {
            curr = curr->left;
        } else if (value > curr->data) {
            curr = curr->right;
        } else {
            return true;
        }
    }
    return false;
}

void RBTree::clear(Node* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

} // namespace dsc

#endif // DSC_RB_TREE_H
