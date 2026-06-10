#include <dsc/array.h>
#include <iostream>
#include <cassert>

int main() {
    dsc::Array<int, 5> arr = {1, 2, 3, 4, 5};
    assert(arr.size() == 5);
    assert(arr[0] == 1 && arr[4] == 5);
    assert(!arr.empty());

    arr.fill(42);
    assert(arr[0] == 42 && arr[3] == 42);

    dsc::Array<int, 3> arr2 = {10, 20, 30};
    arr2.at(1) = 99;
    assert(arr2.at(1) == 99);

    bool caught = false;
    try { arr2.at(100); } catch (const std::out_of_range&) { caught = true; }
    assert(caught);

    assert(arr2.front() == 10);
    assert(arr2.back() == 30);

    dsc::Array<int, 3> a = {1, 2, 3};
    dsc::Array<int, 3> b = {4, 5, 6};
    a.swap(b);
    assert(a[0] == 4 && b[0] == 1);

    std::cout << "test_array: all tests passed!" << std::endl;
    return 0;
}
