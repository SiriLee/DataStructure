#ifndef DSC_ALGORITHM_EXPRESSION_H
#define DSC_ALGORITHM_EXPRESSION_H

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>
#include <cctype>
#include <sstream>

namespace dsc {
namespace algorithm {

// ============================================================
// 表达式求值
// 支持中缀、前缀、后缀表达式的求值和转换
// 运算符：+ - * /，操作数为浮点数
// ============================================================

// 将表达式字符串分解为 token 列表
inline std::vector<std::string> GetTokens(const std::string& expression) {
    std::vector<std::string> tokens;
    std::size_t i = 0;
    while (i < expression.size()) {
        if (expression[i] == ' ') {
            ++i;
            continue;
        }
        if (expression[i] == '+' || expression[i] == '-' ||
            expression[i] == '*' || expression[i] == '/' ||
            expression[i] == '(' || expression[i] == ')') {
            tokens.push_back(std::string(1, expression[i]));
            ++i;
        } else {
            // 数字（支持小数点和负号的特殊情况需调用者处理）
            std::string num;
            while (i < expression.size() &&
                   (std::isdigit(expression[i]) || expression[i] == '.')) {
                num += expression[i];
                ++i;
            }
            tokens.push_back(num);
        }
    }
    return tokens;
}

// 判断是否为操作数（数字）
inline bool IsOperand(const std::string& token) {
    return !token.empty() &&
           (std::isdigit(token[0]) || (token.size() > 1 && token[0] == '-'));
}

// 判断是否为运算符
inline bool IsOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/";
}

// 获取运算符优先级
inline int Precedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

// 计算二元运算
inline double Calculate(double a, const std::string& op, double b) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0.0) throw std::runtime_error("Division by zero");
        return a / b;
    }
    throw std::runtime_error("Unknown operator: " + op);
}

// 后缀表达式求值
inline double PostfixEval(const std::vector<std::string>& tokens) {
    std::stack<double> stk;
    for (const auto& token : tokens) {
        if (IsOperator(token)) {
            double b = stk.top(); stk.pop();
            double a = stk.top(); stk.pop();
            stk.push(Calculate(a, token, b));
        } else {
            stk.push(std::stod(token));
        }
    }
    return stk.top();
}

// 前缀表达式求值
inline double PrefixEval(const std::vector<std::string>& tokens) {
    std::stack<double> stk;
    // 从右到左扫描
    for (int i = static_cast<int>(tokens.size()) - 1; i >= 0; --i) {
        const std::string& token = tokens[i];
        if (IsOperator(token)) {
            double a = stk.top(); stk.pop();
            double b = stk.top(); stk.pop();
            stk.push(Calculate(a, token, b));
        } else {
            stk.push(std::stod(token));
        }
    }
    return stk.top();
}

// 中缀转后缀
inline std::vector<std::string> InfixToPostfix(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    std::stack<std::string> ops;

    for (const auto& token : tokens) {
        if (IsOperand(token)) {
            output.push_back(token);
        } else if (token == "(") {
            ops.push(token);
        } else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }
            if (!ops.empty()) ops.pop();  // 弹出 "("
        } else if (IsOperator(token)) {
            while (!ops.empty() && ops.top() != "(" &&
                   Precedence(ops.top()) >= Precedence(token)) {
                output.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        }
    }

    while (!ops.empty()) {
        output.push_back(ops.top());
        ops.pop();
    }

    return output;
}

// 中缀表达式求值（先转后缀再求值）
inline double InfixEval(const std::vector<std::string>& tokens) {
    auto postfix = InfixToPostfix(tokens);
    return PostfixEval(postfix);
}

} // namespace algorithm
} // namespace dsc

#endif // DSC_ALGORITHM_EXPRESSION_H
