// ============================================================
// test_external_sort.cpp — 外部排序 + 败者树测试
// ============================================================

#include <dsc/loser_tree.h>
#include <dsc/algorithm/external_sort.h>
#include <iostream>
#include <vector>
#include <cassert>
#include <algorithm>

// ==================== LoserTree ====================

void test_loser_tree_k4() {
    dsc::LoserTree<int> t(4);
    t.Build({7, 2, 9, 1});

    assert(t.Winner() == 3);           // 胜者=1（叶子3）

    t.players_[3] = 8; t.Replay(3);   // 替换 1→8
    assert(t.Winner() == 1);           // 胜者=2（叶子1）

    t.players_[1] = 5; t.Replay(1);   // 替换 2→5
    assert(t.Winner() == 1);           // 胜者=5（叶子1）

    t.players_[0] = 3; t.Replay(0);   // 替换 7→3
    assert(t.Winner() == 0);           // 胜者=3（叶子0）

    std::cout << "  LoserTree k=4: OK\n";
}

void test_loser_tree_k1() {
    dsc::LoserTree<int> t(1);
    t.Build({42});
    assert(t.Winner() == 0);
    t.Replay(0);
    assert(t.Winner() == 0);
    std::cout << "  LoserTree k=1: OK\n";
}

void test_loser_tree_extract() {
    // 依次"耗尽"胜者，验证输出升序
    dsc::LoserTree<int> t(8);
    t.Build({15, 8, 3, 12, 6, 1, 9, 4});

    std::vector<int> out;
    for (int i = 0; i < 8; ++i) {
        int w = t.Winner();
        out.push_back(t.players_[w]);
        t.players_[w] = 99999;
        t.Replay(w);
    }
    assert(std::is_sorted(out.begin(), out.end()));
    std::cout << "  LoserTree extract all: OK\n";
}

// ==================== 置换-选择排序 ====================

void test_rs_fits_in_buffer() {
    std::vector<int> data = {5, 3, 8, 1, 9};
    auto runs = dsc::algorithm::ReplacementSelection(data, 10);

    assert(runs.size() == 1);
    assert(runs[0].size() == 5);
    assert(std::is_sorted(runs[0].begin(), runs[0].end()));

    std::cout << "  RS fits in buffer: OK\n";
}

void test_rs_descending() {
    // 降序最坏情况，M=5 应产生多段
    std::vector<int> data = {50, 40, 30, 20, 10, 9, 8, 7, 6, 5};
    auto runs = dsc::algorithm::ReplacementSelection(data, 5);

    assert(runs.size() >= 2);
    int total = 0;
    for (auto& r : runs) {
        assert(std::is_sorted(r.begin(), r.end()));
        total += static_cast<int>(r.size());
    }
    assert(total == static_cast<int>(data.size()));

    std::cout << "  RS descending M=5: OK (" << runs.size() << " runs)\n";
}

void test_rs_random() {
    std::vector<int> data = {
        374, 796, 950, 183, 731, 779, 598, 596, 156, 445,
        629, 232, 811, 997, 833, 343, 938,  85, 569, 622,
        464, 205, 466, 159, 313, 983, 909, 965, 942, 948,
        702, 580, 184, 992, 738, 973, 969, 132,  70, 230,
        723, 159, 834, 386, 935, 241, 630, 442, 717, 885,
        134, 778, 421, 537, 694, 769, 152, 972, 406, 954,
        516, 390, 227, 645, 557, 478, 742, 347, 810, 661,
        290, 827, 525, 384, 603, 149, 419, 870, 268, 718,
        482, 508, 759, 943, 314, 198, 637, 891, 565, 296,
        654, 173, 803, 128, 745, 239, 854, 361, 907, 673
    };
    auto runs = dsc::algorithm::ReplacementSelection(data, 20);
    assert(runs.size() > 1);

    int total = 0;
    for (auto& r : runs) {
        assert(std::is_sorted(r.begin(), r.end()));
        total += static_cast<int>(r.size());
    }
    assert(total == 100);

    std::cout << "  RS random M=20: OK (" << runs.size() << " runs)\n";
}

// ==================== k-路归并 ====================

void test_kway_2() {
    std::vector<std::vector<int>> runs = {{1, 4, 7}, {2, 5, 8}};
    auto result = dsc::algorithm::KWayMerge(runs);
    assert(result.size() == 6);
    assert(std::is_sorted(result.begin(), result.end()));
    std::cout << "  KWayMerge k=2: OK\n";
}

void test_kway_unequal() {
    std::vector<std::vector<int>> runs = {
        {3, 8, 12},
        {1, 5, 9, 15, 20},
        {2, 4, 6, 7}
    };
    auto result = dsc::algorithm::KWayMerge(runs);
    int total = 0;
    for (auto& r : runs) total += static_cast<int>(r.size());
    assert(static_cast<int>(result.size()) == total);
    assert(std::is_sorted(result.begin(), result.end()));
    std::cout << "  KWayMerge unequal: OK\n";
}

// ==================== ExternalSort 端到端 ====================

void test_external_sort_small() {
    std::vector<int> data = {
        83, 42, 17, 95, 61, 28, 74, 39, 50, 12,
        88, 66, 33, 71, 24, 56, 45, 90, 18, 77,
        69, 31, 84, 53, 20, 96, 15, 63, 48, 80
    };
    auto result = dsc::algorithm::ExternalSort(data, 8, 3);

    assert(static_cast<int>(result.size()) == 30);
    assert(std::is_sorted(result.begin(), result.end()));

    auto expected = data;
    std::sort(expected.begin(), expected.end());
    assert(result == expected);

    std::cout << "  ExternalSort N=30 M=8 k=3: OK\n";
}

void test_external_sort_large() {
    std::vector<int> data = {
        374, 796, 950, 183, 731, 779, 598, 596, 156, 445,
        629, 232, 811, 997, 833, 343, 938,  85, 569, 622,
        464, 205, 466, 159, 313, 983, 909, 965, 942, 948,
        702, 580, 184, 992, 738, 973, 969, 132,  70, 230,
        723, 159, 834, 386, 935, 241, 630, 442, 717, 885,
        134, 778, 421, 537, 694, 769, 152, 972, 406, 954,
        516, 390, 227, 645, 557, 478, 742, 347, 810, 661,
        290, 827, 525, 384, 603, 149, 419, 870, 268, 718,
        482, 508, 759, 943, 314, 198, 637, 891, 565, 296,
        654, 173, 803, 128, 745, 239, 854, 361, 907, 673,
        420, 587, 332, 775, 689, 115, 999, 258, 493, 601,
        876, 354, 712, 185, 837, 469, 524, 308, 763, 546,
        680, 272, 899, 142, 612, 395, 733, 487, 848, 211,
        926, 569, 363, 782, 451, 634, 295, 815, 504, 660,
        129, 941, 383, 708, 276, 857, 159, 529, 414, 791,
        343, 670, 234, 882, 458, 625, 323, 750, 196, 865,
        512, 401, 923, 587, 148, 698, 288, 818, 476, 636,
        209, 870, 355, 744, 437, 591, 252, 803, 515, 647,
        124, 965, 392, 718, 266, 841, 483, 614, 301, 776,
        530, 440, 889, 164, 695, 351, 825, 498, 659, 227
    };
    auto result = dsc::algorithm::ExternalSort(data, 25, 5);

    assert(static_cast<int>(result.size()) == 200);
    assert(std::is_sorted(result.begin(), result.end()));

    auto expected = data;
    std::sort(expected.begin(), expected.end());
    assert(result == expected);

    std::cout << "  ExternalSort N=200 M=25 k=5: OK\n";
}

void test_external_sort_single_run() {
    std::vector<int> data = {9, 3, 7, 1, 5};
    auto result = dsc::algorithm::ExternalSort(data, 10, 3);
    assert(result.size() == 5);
    assert(std::is_sorted(result.begin(), result.end()));
    std::cout << "  ExternalSort single run: OK\n";
}

void test_external_sort_empty() {
    auto result = dsc::algorithm::ExternalSort({}, 5, 3);
    assert(result.empty());
    std::cout << "  ExternalSort empty: OK\n";
}

// ==================== 主入口 ====================

int main() {
    std::cout << "=== test_external_sort ===\n";

    std::cout << "[LoserTree]\n";
    test_loser_tree_k4();
    test_loser_tree_k1();
    test_loser_tree_extract();

    std::cout << "[ReplacementSelection]\n";
    test_rs_fits_in_buffer();
    test_rs_descending();
    test_rs_random();

    std::cout << "[KWayMerge]\n";
    test_kway_2();
    test_kway_unequal();

    std::cout << "[ExternalSort]\n";
    test_external_sort_small();
    test_external_sort_large();
    test_external_sort_single_run();
    test_external_sort_empty();

    std::cout << "\ntest_external_sort: all tests passed!\n";
    return 0;
}
