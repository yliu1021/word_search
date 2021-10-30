add_subdirectory(trie)
add_subdirectory(solver)

add_executable(word_search main.cpp)
target_link_libraries(word_search trie)
