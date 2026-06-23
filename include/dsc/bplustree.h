#ifndef DSC_BPLUSTREE_H
#define DSC_BPLUSTREE_H

#include <vector>
#include <utility>

namespace dsc {

/// m-order B+ tree (search & insert only, int keys).
/// All keys reside in leaf nodes; internal nodes only route searches.
/// Leaves are linked via next_ for sequential traversal.
/// m = maximum number of keys per node (split when size reaches m).
template<int m>
class BPlusTree {
    static_assert(m >= 2, "B+ tree order must be at least 2");

public:
    BPlusTree() = default;

    ~BPlusTree() {
        clear_(root_);
    }

    /// Returns true if key exists in the tree.
    /// Always descends to leaf level.
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
        Node*              next_;

        Node() : is_leaf_(true), next_(nullptr) {}
    };

    Node* root_ = nullptr;

    void clear_(Node* node);

    /// Recursively inserts key into subtree rooted at node.
    /// Returns {promoted_key, right_sibling} when a split propagates;
    /// returns {0, nullptr} when no split occurs.
    std::pair<int, Node*> insert_(Node* node, int key);

    /// Splits a leaf node. mid = m / 2.
    /// First key of right leaf is copied up (still present in right leaf).
    std::pair<int, Node*> split_leaf_(Node* node);

    /// Splits an internal node. mid = (m - 1) / 2.
    /// Median key is moved up (removed from both children).
    std::pair<int, Node*> split_internal_(Node* node);
};

// ================================================================
// Implementation
// ================================================================

template<int m>
bool BPlusTree<m>::Search(int key) const {
    Node* node = root_;
    while (node != nullptr) {
        if (node->is_leaf_) {
            // Leaf — check for exact match
            size_t i = 0;
            while (i < node->keys_.size() && node->keys_[i] < key) {
                ++i;
            }
            return i < node->keys_.size() && node->keys_[i] == key;
        }
        // Internal node: go right on equality (data lives in leaves)
        size_t pos = 0;
        while (pos < node->keys_.size() && node->keys_[pos] <= key) {
            ++pos;
        }
        node = node->children_[pos];
    }
    return false;
}

template<int m>
bool BPlusTree<m>::Insert(int key) {
    // Reject duplicates
    if (Search(key)) {
        return false;
    }

    // Empty tree — create root (a leaf) directly
    if (root_ == nullptr) {
        root_ = new Node();
        root_->keys_.push_back(key);
        return true;
    }

    auto [promoted_key, right_sibling] = insert_(root_, key);

    // Split propagated to root — create new root
    if (right_sibling != nullptr) {
        Node* new_root = new Node();
        new_root->is_leaf_ = false;
        new_root->keys_.push_back(promoted_key);
        new_root->children_.push_back(root_);
        new_root->children_.push_back(right_sibling);
        root_ = new_root;
    }

    return true;
}

template<int m>
void BPlusTree<m>::clear_(typename BPlusTree<m>::Node* node) {
    if (node == nullptr) {
        return;
    }
    for (Node* child : node->children_) {
        clear_(child);
    }
    delete node;
}

template<int m>
std::pair<int, typename BPlusTree<m>::Node*>
BPlusTree<m>::insert_(typename BPlusTree<m>::Node* node, int key) {
    if (node->is_leaf_) {
        // Find insertion position (sorted insert)
        size_t pos = 0;
        while (pos < node->keys_.size() && node->keys_[pos] < key) {
            ++pos;
        }
        node->keys_.insert(node->keys_.begin() + pos, key);

        // Check leaf overflow
        return split_leaf_(node);
    } else {
        // Find child: go right of equal keys (upper_bound semantics)
        size_t pos = 0;
        while (pos < node->keys_.size() && node->keys_[pos] <= key) {
            ++pos;
        }

        auto [promoted_key, right_sibling] = insert_(node->children_[pos], key);

        // Child split — absorb promoted key and right sibling
        if (right_sibling != nullptr) {
            node->keys_.insert(node->keys_.begin() + pos, promoted_key);
            node->children_.insert(node->children_.begin() + pos + 1, right_sibling);
        }

        // Check internal overflow
        return split_internal_(node);
    }
}

template<int m>
std::pair<int, typename BPlusTree<m>::Node*>
BPlusTree<m>::split_leaf_(typename BPlusTree<m>::Node* node) {
    if (node->keys_.size() < static_cast<size_t>(m)) {
        return {0, nullptr};  // no overflow
    }

    // Leaf split: mid = m / 2
    const int mid = m / 2;

    Node* right = new Node();
    right->is_leaf_ = true;

    // Move keys[mid .. end] to right leaf
    right->keys_.assign(node->keys_.begin() + mid, node->keys_.end());

    // Shrink current node: keep keys[0 .. mid-1]
    node->keys_.resize(mid);

    // Link leaves
    right->next_ = node->next_;
    node->next_ = right;

    // COPY UP: first key of right leaf (still present in right leaf)
    int promoted_key = right->keys_[0];

    return {promoted_key, right};
}

template<int m>
std::pair<int, typename BPlusTree<m>::Node*>
BPlusTree<m>::split_internal_(typename BPlusTree<m>::Node* node) {
    if (node->keys_.size() < static_cast<size_t>(m)) {
        return {0, nullptr};  // no overflow
    }

    // Internal split: mid = (m - 1) / 2, same as B-tree
    const int mid = (m - 1) / 2;
    int median = node->keys_[mid];

    Node* right = new Node();
    right->is_leaf_ = false;

    // Move keys[mid+1 .. end] to right sibling
    right->keys_.assign(node->keys_.begin() + mid + 1, node->keys_.end());

    // Move children[mid+1 .. end] to right sibling
    right->children_.assign(node->children_.begin() + mid + 1,
                            node->children_.end());

    // Shrink current node: keep keys[0..mid-1] and children[0..mid]
    node->keys_.resize(mid);
    node->children_.resize(mid + 1);

    // MOVE UP: median is removed from both children
    return {median, right};
}

} // namespace dsc

#endif // DSC_BPLUSTREE_H
