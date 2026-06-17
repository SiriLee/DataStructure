#include <dsc/trie.h>
#include <iostream>
#include <cassert>
#include <string>

int main() {
    // Insert + Search
    {
        dsc::Trie trie;
        trie.Insert("hello");
        trie.Insert("world");

        assert(trie.Search("hello"));
        assert(trie.Search("world"));
        assert(!trie.Search("hell"));
        assert(!trie.Search("worlds"));
    }

    // StartsWith
    {
        dsc::Trie trie;
        trie.Insert("apple");
        trie.Insert("app");

        assert(trie.StartsWith("app"));
        assert(trie.StartsWith("a"));
        assert(trie.StartsWith("apple"));
        assert(!trie.StartsWith("b"));
        assert(!trie.StartsWith("apples"));
    }

    // Insert duplicate is idempotent
    {
        dsc::Trie trie;
        trie.Insert("abc");
        trie.Insert("abc");
        assert(trie.Search("abc"));
        assert(trie.Remove("abc"));
        assert(!trie.Search("abc"));
    }

    // Remove
    {
        dsc::Trie trie;
        trie.Insert("cat");
        trie.Insert("car");

        assert(trie.Search("cat"));
        assert(trie.Remove("cat"));
        assert(!trie.Search("cat"));
        assert(trie.Search("car"));  // "car" still exists
    }

    // Remove cascade — delete nodes no longer needed
    {
        dsc::Trie trie;
        trie.Insert("abc");

        assert(trie.Search("abc"));
        assert(trie.Remove("abc"));
        assert(!trie.Search("abc"));
        assert(!trie.StartsWith("a"));  // all nodes were cleaned up
    }

    // Remove when prefix exists as separate word
    {
        dsc::Trie trie;
        trie.Insert("app");
        trie.Insert("apple");

        assert(trie.Remove("apple"));
        assert(!trie.Search("apple"));
        assert(trie.Search("app"));     // "app" still exists
        assert(trie.StartsWith("app")); // prefix "app" still exists
    }

    // Remove non-existent word
    {
        dsc::Trie trie;
        trie.Insert("abc");
        assert(!trie.Remove("xyz"));
        assert(!trie.Remove("ab"));
    }

    // Empty string
    {
        dsc::Trie trie;
        trie.Insert("");
        assert(trie.Search(""));
        assert(trie.StartsWith(""));
        assert(trie.Remove(""));
        assert(!trie.Search(""));
    }

    // Search / StartsWith on empty trie
    {
        dsc::Trie trie;
        assert(!trie.Search("a"));
        assert(!trie.StartsWith("a"));
        assert(trie.StartsWith(""));   // root always exists
        assert(!trie.Search(""));      // root is not a word end
    }

    std::cout << "test_trie: all tests passed!" << std::endl;
    return 0;
}
