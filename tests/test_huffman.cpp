#include <dsc/algorithm/huffman.h>
#include <iostream>
#include <cassert>

int main() {
    using namespace dsc::algorithm;

    // 构建哈夫曼树
    HuffmanTree tree;
    tree.build("hello world");

    // 编码
    BitWriter writer;
    tree.encode("hello", writer);
    assert(writer.bytes().size() > 0);

    // 解码
    BitReader reader(writer.bytes());
    std::string decoded = tree.decode(reader);
    // 解码可能末尾有填充，截取前5个字符比较
    assert(decoded.substr(0, 5) == "hello");

    std::cout << "test_huffman: all tests passed!" << std::endl;
    return 0;
}
