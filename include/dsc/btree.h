#ifndef DSC_BTREE_H
#define DSC_BTREE_H

#include <vector>
#include <utility>

namespace dsc {

/// m-order B-tree (search & insert only, int keys).
/// m = maximum number of children per node; minimum = ceil(m/2).
template<int m>
class BTree {
    static_assert(m >= 2, "B-tree order must be at least 2");

public:
    BTree() = default;

    ~BTree() {
        clear_(root_);
    }

    /// Returns true if key exists in the tree.
    bool Search(int key) const;

    /// Inserts key. Returns false if key already exists (no duplicates).
    bool Insert(int key);

    /// Returns true if the tree is empty.
    bool Empty() const { return root_ == nullptr; }

private:
    struct Node {
        std::vector<int>   keys_;
        std::vector<Node*> children_;
        bool               is_leaf_;

        Node() : is_leaf_(true) {}
    };

    Node* root_ = nullptr;

    void clear_(Node* node);

    /// Recursively inserts key into subtree rooted at node.
    /// Returns {median, right_sibling} when a split propagates upward;
    /// returns {0, nullptr} when no split occurs.
    std::pair<int, Node*> insert_(Node* node, int key);

    /// Splits node when keys_.size() == m.
    /// Returns {median, new_right_node}; or {0, nullptr} if no overflow.
    std::pair<int, Node*> split_overflow_(Node* node);
};

// ================================================================
// Implementation
// ================================================================

template<int m>
bool BTree<m>::Search(int key) const {
    Node* node = root_;
    while (node != nullptr) {
        // Linear search to find position
        size_t i = 0;
        while (i < node->keys_.size() && node->keys_[i] < key) {
            ++i;
        }
        if (i < node->keys_.size() && node->keys_[i] == key) {
            return true;
        }
        if (node->is_leaf_) {
            return false;
        }
        node = node->children_[i];
    }
    return false;
}

template<int m>
bool BTree<m>::Insert(int key) {
    // Reject duplicates
    if (Search(key)) {
        return false;
    }

    // Empty tree — create root directly
    if (root_ == nullptr) {
        root_ = new Node();
        root_->keys_.push_back(key);
        return true;
    }

    auto [median, right_sibling] = insert_(root_, key);

    // Split propagated to root — create new root
    if (right_sibling != nullptr) {
        Node* new_root = new Node();
        new_root->is_leaf_ = false;
        new_root->keys_.push_back(median);
        new_root->children_.push_back(root_);
        new_root->children_.push_back(right_sibling);
        root_ = new_root;
    }

    return true;
}

template<int m>
void BTree<m>::clear_(typename BTree<m>::Node* node) {
    if (node == nullptr) {
        return;
    }
    for (Node* child : node->children_) {
        clear_(child);
    }
    delete node;
}

template<int m>
std::pair<int, typename BTree<m>::Node*> BTree<m>::insert_(typename BTree<m>::Node* node, int key) {
    // Find insertion / descent position
    size_t pos = 0;
    while (pos < node->keys_.size() && node->keys_[pos] < key) {
        ++pos;
    }

    if (node->is_leaf_) {
        // Insert key into leaf
        node->keys_.insert(node->keys_.begin() + pos, key);
    } else {
        // Descend into child
        auto [median, right_sibling] = insert_(node->children_[pos], key);

        // Child split — absorb median and right sibling
        if (right_sibling != nullptr) {
            node->keys_.insert(node->keys_.begin() + pos, median);
            node->children_.insert(node->children_.begin() + pos + 1, right_sibling);
        }
    }

    // Check overflow and split if needed
    return split_overflow_(node);
}

template<int m>
std::pair<int, typename BTree<m>::Node*> BTree<m>::split_overflow_(typename BTree<m>::Node* node) {
    if (node->keys_.size() < static_cast<size_t>(m)) {
        return {0, nullptr};  // no overflow
    }

    // keys_.size() == m → overflow, split at mid
    const int mid = (m - 1) / 2;
    int median = node->keys_[mid];

    Node* right = new Node();
    right->is_leaf_ = node->is_leaf_;

    // Move keys[mid+1 .. end] to right sibling
    right->keys_.assign(node->keys_.begin() + mid + 1, node->keys_.end());

    // Move children[mid+1 .. end] if internal node
    if (!node->is_leaf_) {
        right->children_.assign(node->children_.begin() + mid + 1, node->children_.end());
    }

    // Shrink current node: keep keys[0..mid-1] and children[0..mid]
    node->keys_.resize(mid);
    if (!node->is_leaf_) {
        node->children_.resize(mid + 1);
    }

    return {median, right};
}

} // namespace dsc

#endif // DSC_BTREE_H
