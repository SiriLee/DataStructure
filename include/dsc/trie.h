#ifndef DSC_TRIE_H
#define DSC_TRIE_H

#include <string>

namespace dsc {

// ============================================================
// Trie — 字典树（前缀树）
// 支持小写字母 a-z，每个节点 26 个子节点 + 结束标记
// ============================================================
class Trie {
private:
    struct Node {
        Node* children_[26];
        bool is_end_;

        Node() : is_end_(false) {
            for (int i = 0; i < 26; ++i) {
                children_[i] = nullptr;
            }
        }
    };

    Node* root_;

    void clear_(Node* node);
    bool remove_(Node* node, const std::string& word, int depth);
    bool is_empty_(Node* node) const;

public:
    Trie() : root_(new Node()) {}
    ~Trie() { clear_(root_); }

    void Insert(const std::string& word);
    bool Search(const std::string& word) const;
    bool StartsWith(const std::string& prefix) const;
    bool Remove(const std::string& word);
};

// ==================== 实现 ====================

inline void Trie::clear_(Node* node) {
    if (!node) return;
    for (int i = 0; i < 26; ++i) {
        clear_(node->children_[i]);
    }
    delete node;
}

inline bool Trie::is_empty_(Node* node) const {
    for (int i = 0; i < 26; ++i) {
        if (node->children_[i]) return false;
    }
    return true;
}

inline void Trie::Insert(const std::string& word) {
    Node* cur = root_;
    for (char ch : word) {
        int idx = ch - 'a';
        if (!cur->children_[idx]) {
            cur->children_[idx] = new Node();
        }
        cur = cur->children_[idx];
    }
    cur->is_end_ = true;
}

inline bool Trie::Search(const std::string& word) const {
    Node* cur = root_;
    for (char ch : word) {
        int idx = ch - 'a';
        if (!cur->children_[idx]) return false;
        cur = cur->children_[idx];
    }
    return cur->is_end_;
}

inline bool Trie::StartsWith(const std::string& prefix) const {
    Node* cur = root_;
    for (char ch : prefix) {
        int idx = ch - 'a';
        if (!cur->children_[idx]) return false;
        cur = cur->children_[idx];
    }
    return true;
}

inline bool Trie::Remove(const std::string& word) {
    return remove_(root_, word, 0);
}

inline bool Trie::remove_(Node* node, const std::string& word, int depth) {
    if (!node) return false;

    if (depth == static_cast<int>(word.size())) {
        if (!node->is_end_) return false;
        node->is_end_ = false;
        return true;
    }

    int idx = word[depth] - 'a';
    Node* child = node->children_[idx];
    if (!child) return false;

    if (!remove_(child, word, depth + 1)) return false;

    if (!child->is_end_ && is_empty_(child)) {
        delete child;
        node->children_[idx] = nullptr;
    }
    return true;
}

} // namespace dsc

#endif // DSC_TRIE_H
