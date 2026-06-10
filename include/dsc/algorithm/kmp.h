#ifndef DSC_ALGORITHM_KMP_H
#define DSC_ALGORITHM_KMP_H

#include <string>
#include <vector>

namespace dsc {
namespace algorithm {

// ============================================================
// KMP 字符串匹配算法
// ============================================================

// 计算 next 数组（原始版本）
// next[i] 表示 pattern[0..i-1] 的最长相等前后缀长度
inline std::vector<int> GetNext(const std::string& pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<int> next(m, 0);
    next[0] = -1;
    int i = 0, j = -1;
    while (i < m - 1) {
        if (j == -1 || pattern[i] == pattern[j]) {
            ++i; ++j;
            next[i] = j;
        } else {
            j = next[j];
        }
    }
    return next;
}

// KMP 搜索（使用原始 next 数组）
// 返回 pattern 在 text 中第一次出现的位置，找不到返回 -1
inline int KmpSearch(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return 0;
    auto next = GetNext(pattern);
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    int i = 0, j = 0;
    while (i < n) {
        if (j == -1 || text[i] == pattern[j]) {
            ++i; ++j;
        } else {
            j = next[j];
        }
        if (j == m) {
            return i - j;  // 匹配成功，返回起始位置
        }
    }
    return -1;
}

// 计算修正后的 next 数组（优化版本）
// 当 pattern[i] == pattern[j] 时进一步优化，避免无效比较
inline std::vector<int> GetFixedNext(const std::string& pattern) {
    int m = static_cast<int>(pattern.size());
    std::vector<int> next(m, 0);
    next[0] = -1;
    int i = 0, j = -1;
    while (i < m - 1) {
        if (j == -1 || pattern[i] == pattern[j]) {
            ++i; ++j;
            if (pattern[i] != pattern[j]) {
                next[i] = j;
            } else {
                next[i] = next[j];
            }
        } else {
            j = next[j];
        }
    }
    return next;
}

// KMP 搜索（使用修正 next 数组）
inline int KmpSearchFixed(const std::string& text, const std::string& pattern) {
    if (pattern.empty()) return 0;
    auto next = GetFixedNext(pattern);
    int n = static_cast<int>(text.size());
    int m = static_cast<int>(pattern.size());
    int i = 0, j = 0;
    while (i < n) {
        if (j == -1 || text[i] == pattern[j]) {
            ++i; ++j;
        } else {
            j = next[j];
        }
        if (j == m) {
            return i - j;
        }
    }
    return -1;
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_KMP_H
