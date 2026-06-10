#ifndef DSC_ALGORITHM_ROLLING_HASH_H
#define DSC_ALGORITHM_ROLLING_HASH_H

#include <string>
#include <vector>
#include <utility>

namespace dsc {
namespace algorithm {

// ============================================================
// 滚动哈希（Rolling Hash / Rabin-Karp）
// 用于快速字符串匹配和子串哈希查询
// ============================================================

// -------------------- 单哈希版本 --------------------
class RollingHash {
public:
    explicit RollingHash(const std::string& s);

    // 获取子串 s[l..r] 的哈希值（闭区间）
    long long get_hash(int l, int r) const;

    // 获取整个字符串的哈希值
    long long get_hash() const;

    // 在文本中查找 pattern，返回首次出现位置，找不到返回 npos
    std::size_t find(const std::string& pattern) const;

private:
    static const int BASE = 131;
    static const int MOD  = 1000000007;  // 1e9 + 7

    std::string           text_;
    std::vector<long long> prefix_hash_;  // 前缀哈希
    std::vector<long long> pow_base_;     // BASE 的幂
};

inline RollingHash::RollingHash(const std::string& s) : text_(s) {
    int n = static_cast<int>(s.size());
    prefix_hash_.resize(n + 1, 0);
    pow_base_.resize(n + 1, 1);

    for (int i = 0; i < n; ++i) {
        prefix_hash_[i + 1] = (prefix_hash_[i] * BASE % MOD + s[i]) % MOD;
        pow_base_[i + 1] = (pow_base_[i] * BASE) % MOD;
    }
}

inline long long RollingHash::get_hash(int l, int r) const {
    long long result = prefix_hash_[r + 1] -
                       (prefix_hash_[l] * pow_base_[r - l + 1] % MOD);
    return (result + MOD) % MOD;
}

inline long long RollingHash::get_hash() const {
    return prefix_hash_.back();
}

inline std::size_t RollingHash::find(const std::string& pattern) const {
    int n = static_cast<int>(text_.size());
    int m = static_cast<int>(pattern.size());
    if (m == 0) return 0;
    if (m > n) return static_cast<std::size_t>(-1);

    // 计算 pattern 的哈希值
    RollingHash pattern_hash(pattern);
    long long target = pattern_hash.get_hash();

    // 滑动窗口匹配
    for (int i = 0; i <= n - m; ++i) {
        if (get_hash(i, i + m - 1) == target) {
            // 验证（避免哈希碰撞）
            if (text_.substr(i, m) == pattern) {
                return static_cast<std::size_t>(i);
            }
        }
    }
    return static_cast<std::size_t>(-1);
}

// -------------------- 双哈希版本（减少碰撞） --------------------
class RollingHashPlus {
public:
    explicit RollingHashPlus(const std::string& s);

    // 返回 (hash1, hash2)
    std::pair<long long, long long> get_hash(int l, int r) const;
    std::pair<long long, long long> get_hash() const;

    std::size_t find(const std::string& pattern) const;

private:
    static const int BASE1 = 131, MOD1 = 1000000007;
    static const int BASE2 = 137, MOD2 = 1000000009;

    std::string text_;
    std::vector<long long> pre1_, pow1_;
    std::vector<long long> pre2_, pow2_;
};

inline RollingHashPlus::RollingHashPlus(const std::string& s) : text_(s) {
    int n = static_cast<int>(s.size());
    pre1_.resize(n + 1, 0); pow1_.resize(n + 1, 1);
    pre2_.resize(n + 1, 0); pow2_.resize(n + 1, 1);

    for (int i = 0; i < n; ++i) {
        pre1_[i + 1] = (pre1_[i] * BASE1 % MOD1 + s[i]) % MOD1;
        pow1_[i + 1] = (pow1_[i] * BASE1) % MOD1;
        pre2_[i + 1] = (pre2_[i] * BASE2 % MOD2 + s[i]) % MOD2;
        pow2_[i + 1] = (pow2_[i] * BASE2) % MOD2;
    }
}

inline std::pair<long long, long long>
RollingHashPlus::get_hash(int l, int r) const {
    long long h1 = (pre1_[r + 1] - pre1_[l] * pow1_[r - l + 1] % MOD1 + MOD1) % MOD1;
    long long h2 = (pre2_[r + 1] - pre2_[l] * pow2_[r - l + 1] % MOD2 + MOD2) % MOD2;
    return {h1, h2};
}

inline std::pair<long long, long long>
RollingHashPlus::get_hash() const {
    return {pre1_.back(), pre2_.back()};
}

inline std::size_t RollingHashPlus::find(const std::string& pattern) const {
    int n = static_cast<int>(text_.size());
    int m = static_cast<int>(pattern.size());
    if (m == 0) return 0;
    if (m > n) return static_cast<std::size_t>(-1);

    RollingHashPlus ph(pattern);
    auto target = ph.get_hash();

    for (int i = 0; i <= n - m; ++i) {
        if (get_hash(i, i + m - 1) == target) {
            if (text_.substr(i, m) == pattern) {
                return static_cast<std::size_t>(i);
            }
        }
    }
    return static_cast<std::size_t>(-1);
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_ROLLING_HASH_H
