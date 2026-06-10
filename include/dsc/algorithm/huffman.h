#ifndef DSC_ALGORITHM_HUFFMAN_H
#define DSC_ALGORITHM_HUFFMAN_H

#include <string>
#include <unordered_map>
#include <queue>
#include <vector>
#include <cstddef>
#include <fstream>

namespace dsc {
namespace algorithm {

// ============================================================
// Huffman 编码
// 支持从文本构建哈夫曼树，编码为比特流，以及解码
// ============================================================

// 简易比特写入器
class BitWriter {
public:
    BitWriter() = default;

    void write_bit(bool bit) {
        buffer_ = (buffer_ << 1) | (bit ? 1 : 0);
        ++bit_count_;
        if (bit_count_ == 8) {
            bytes_.push_back(static_cast<char>(buffer_));
            buffer_ = 0;
            bit_count_ = 0;
        }
    }

    void flush() {
        if (bit_count_ > 0) {
            buffer_ <<= (8 - bit_count_);
            bytes_.push_back(static_cast<char>(buffer_));
            buffer_ = 0;
            bit_count_ = 0;
        }
    }

    const std::vector<char>& bytes() const { return bytes_; }
    std::size_t total_bits() const { return bytes_.size() * 8; }

private:
    std::vector<char> bytes_;
    unsigned char buffer_ = 0;
    int bit_count_ = 0;
};

// 简易比特读取器
class BitReader {
public:
    BitReader(const std::vector<char>& data)
        : data_(data), byte_pos_(0), bit_pos_(0) {}

    bool read_bit() {
        if (byte_pos_ >= data_.size()) return false;
        bool bit = (data_[byte_pos_] >> (7 - bit_pos_)) & 1;
        ++bit_pos_;
        if (bit_pos_ == 8) {
            bit_pos_ = 0;
            ++byte_pos_;
        }
        return bit;
    }

    bool eof() const {
        return byte_pos_ >= data_.size();
    }

private:
    const std::vector<char>& data_;
    std::size_t byte_pos_;
    int bit_pos_;
};

// ============================================================
// HuffmanTree — 哈夫曼树
// ============================================================
class HuffmanTree {
private:
    struct Node {
        char  data;
        int   freq;
        Node* left;
        Node* right;

        Node(char d, int f)
            : data(d), freq(f), left(nullptr), right(nullptr) {}
    };

    Node* root_ = nullptr;
    std::unordered_map<char, std::string> codes_;  // 字符 -> 编码

    void clear_(Node* node);
    void generate_codes_(Node* node, const std::string& code);

    // 用于优先队列的比较器
    struct Compare {
        bool operator()(Node* a, Node* b) {
            return a->freq > b->freq;
        }
    };

public:
    HuffmanTree() = default;
    ~HuffmanTree() { clear_(root_); }

    // 由频率表构建
    void build(const std::unordered_map<char, int>& freq_map);

    // 由文本直接构建
    void build(const std::string& text);

    // 编码
    void encode(const std::string& text, BitWriter& writer);

    // 解码
    std::string decode(BitReader& reader);

    // 获取编码表
    const std::unordered_map<char, std::string>& codes() const { return codes_; }
};

// ==================== 实现 ====================

inline void HuffmanTree::clear_(Node* node) {
    if (!node) return;
    clear_(node->left);
    clear_(node->right);
    delete node;
}

inline void HuffmanTree::generate_codes_(Node* node, const std::string& code) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes_[node->data] = code.empty() ? "0" : code;  // 单字符情况
    }
    generate_codes_(node->left, code + "0");
    generate_codes_(node->right, code + "1");
}

inline void HuffmanTree::build(const std::unordered_map<char, int>& freq_map) {
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;

    for (const auto& pair : freq_map) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node* right = pq.top(); pq.pop();
        Node* parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    root_ = pq.empty() ? nullptr : pq.top();
    codes_.clear();
    generate_codes_(root_, "");
}

inline void HuffmanTree::build(const std::string& text) {
    std::unordered_map<char, int> freq_map;
    for (char ch : text) {
        ++freq_map[ch];
    }
    build(freq_map);
}

inline void HuffmanTree::encode(const std::string& text, BitWriter& writer) {
    for (char ch : text) {
        const std::string& code = codes_.at(ch);
        for (char bit : code) {
            writer.write_bit(bit == '1');
        }
    }
    writer.flush();
}

inline std::string HuffmanTree::decode(BitReader& reader) {
    std::string result;
    if (!root_) return result;
    // 单字符特殊情况
    if (!root_->left && !root_->right) {
        while (!reader.eof()) {
            reader.read_bit();
            result += root_->data;
        }
        return result;
    }
    while (!reader.eof()) {
        Node* cur = root_;
        while (cur->left || cur->right) {
            if (reader.eof()) return result;
            bool bit = reader.read_bit();
            cur = bit ? cur->right : cur->left;
        }
        result += cur->data;
    }
    return result;
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_HUFFMAN_H
