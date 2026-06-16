#ifndef DSC_RB_TREE_H
#define DSC_RB_TREE_H

namespace dsc {

// ============================================================
// RBTree — 红黑树
// 实现：搜索、插入、删除
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
    void remove(int value);
    bool search(int value) const;

private:
    // 旋转（含父指针维护）
    Node* rotate_left(Node* x);
    Node* rotate_right(Node* y);

    // 插入修正：修复连续红色违规
    void insert_fixup(Node* node);

    // 删除修正：修复黑色高度违规
    void remove_fixup(Node* node, Node* parent);

    // 辅助
    Node* minimum(Node* node) const;
    void transplant(Node* u, Node* v);
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

// ==================== 删除 ====================

void RBTree::remove(int value) {
    // 查找待删除节点
    Node* node = root_;
    while (node) {
        if (value < node->data) {
            node = node->left;
        } else if (value > node->data) {
            node = node->right;
        } else {
            break;
        }
    }
    if (!node) return; // 未找到

    Node* y = node;           // 实际被移除（或移动）的节点
    Node* x = nullptr;        // 替换 y 的节点
    Node* x_parent = nullptr; // x 的父节点
    Color y_original = y->color;

    if (!node->left) {
        // 只有右子（或无子）
        x = node->right;
        x_parent = node->parent;
        transplant(node, node->right);
    } else if (!node->right) {
        // 只有左子
        x = node->left;
        x_parent = node->parent;
        transplant(node, node->left);
    } else {
        // 有两个子节点：用后继替换
        y = minimum(node->right);
        y_original = y->color;
        x = y->right;

        if (y->parent == node) {
            x_parent = y;
        } else {
            transplant(y, y->right);
            y->right = node->right;
            y->right->parent = y;
            x_parent = y->parent;
        }

        transplant(node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    delete node;

    // 若移除的是黑节点，需修正
    if (y_original == BLACK) {
        remove_fixup(x, x_parent);
    }
}

void RBTree::remove_fixup(Node* node, Node* parent) {
    // node 可能为空（双重黑色在空节点上）
    while (node != root_ && (!node || node->color == BLACK)) {
        if (node == parent->left) {
            Node* sibling = parent->right;

            // 情况1：兄弟为红
            if (sibling && sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotate_left(parent);
                sibling = parent->right;
            }

            // 情况2：兄弟两子皆黑
            if ((!sibling->left || sibling->left->color == BLACK) &&
                (!sibling->right || sibling->right->color == BLACK)) {
                if (sibling) sibling->color = RED;
                node = parent;
                parent = node->parent;
            } else {
                // 情况3：兄弟左子红、右子黑
                if (!sibling->right || sibling->right->color == BLACK) {
                    if (sibling->left) sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotate_right(sibling);
                    sibling = parent->right;
                }
                // 情况4：兄弟右子红
                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->right) sibling->right->color = BLACK;
                rotate_left(parent);
                node = root_;
            }
        } else {
            // 对称：node 是右子
            Node* sibling = parent->left;

            if (sibling && sibling->color == RED) {
                sibling->color = BLACK;
                parent->color = RED;
                rotate_right(parent);
                sibling = parent->left;
            }

            if ((!sibling->left || sibling->left->color == BLACK) &&
                (!sibling->right || sibling->right->color == BLACK)) {
                if (sibling) sibling->color = RED;
                node = parent;
                parent = node->parent;
            } else {
                if (!sibling->left || sibling->left->color == BLACK) {
                    if (sibling->right) sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotate_left(sibling);
                    sibling = parent->left;
                }
                sibling->color = parent->color;
                parent->color = BLACK;
                if (sibling->left) sibling->left->color = BLACK;
                rotate_right(parent);
                node = root_;
            }
        }
    }
    if (node) node->color = BLACK;
}

// ==================== 辅助函数 ====================

void RBTree::transplant(Node* u, Node* v) {
    // 用 v 替换 u
    if (!u->parent) {
        root_ = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) v->parent = u->parent;
}

RBTree::Node* RBTree::minimum(Node* node) const {
    while (node && node->left) node = node->left;
    return node;
}

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
