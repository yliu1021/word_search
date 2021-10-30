add_library(solver solver.cpp grid.cpp)
target_link_libraries(solver PUBLIC trie)
target_include_directories(solver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
