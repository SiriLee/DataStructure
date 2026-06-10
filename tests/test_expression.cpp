#include <dsc/algorithm/expression.h>
#include <iostream>
#include <cassert>
#include <cmath>

int main() {
    using namespace dsc::algorithm;

    // 后缀表达式
    {
        auto tokens = GetTokens("3 4 + 5 *");
        // tokens: {"3", "4", "+", "5", "*"}
        // But GetTokens handles space-separated, let's manually construct
        std::vector<std::string> postfix = {"3", "4", "+", "5", "*"};
        double result = PostfixEval(postfix);
        assert(std::abs(result - 35.0) < 1e-9);
    }

    // 前缀表达式
    {
        std::vector<std::string> prefix = {"*", "+", "3", "4", "5"};
        double result = PrefixEval(prefix);
        assert(std::abs(result - 35.0) < 1e-9);
    }

    // 中缀转后缀
    {
        auto tokens = GetTokens("3+4*5");
        auto postfix = InfixToPostfix(tokens);
        double result = PostfixEval(postfix);
        assert(std::abs(result - 23.0) < 1e-9);
    }

    // 中缀直接求值
    {
        auto tokens = GetTokens("(3+4)*5");
        double result = InfixEval(tokens);
        assert(std::abs(result - 35.0) < 1e-9);
    }

    std::cout << "test_expression: all tests passed!" << std::endl;
    return 0;
}
