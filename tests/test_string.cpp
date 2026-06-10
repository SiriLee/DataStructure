#include <dsc/string.h>
#include <iostream>
#include <cassert>

int main() {
    dsc::String s1;
    assert(s1.empty() && s1.size() == 0);

    dsc::String s2("hello");
    assert(s2.size() == 5);
    assert(s2 == "hello");

    s2 += " world";
    assert(s2 == "hello world");

    s2.push_back('!');
    assert(s2.back() == '!');

    s2.pop_back();
    assert(s2.back() == 'd');

    dsc::String s3 = s2;
    assert(s3 == s2);

    dsc::String s4 = std::move(s3);
    assert(s4 == "hello world");
    assert(s3.empty());

    // find
    assert(s4.find("world") == 6);
    assert(s4.find('o') == 4);
    assert(s4.rfind('o') == 7);

    // insert
    s4.insert(5, " beautiful");
    assert(s4 == "hello beautiful world");

    // erase
    s4.erase(5, 10);
    assert(s4 == "hello world");

    std::cout << "test_string: all tests passed!" << std::endl;
    return 0;
}
