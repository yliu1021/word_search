add_executable(trie_tests trie_tests.cpp)
target_link_libraries(trie_tests gtest_main trie)
gtest_discover_tests(trie_tests)
