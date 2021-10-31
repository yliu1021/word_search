//
// Created by Yuhan Liu on 10/30/21.
//

#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

#include "grid.h"
#include "solver.h"
#include "trie.h"

auto adjacent(const Grid::Pos &a, const Grid::Pos &b) -> bool {
  auto ax = static_cast<int64_t>(a.first);
  auto ay = static_cast<int64_t>(a.second);
  auto bx = static_cast<int64_t>(b.first);
  auto by = static_cast<int64_t>(b.second);
  return std::abs(ax - bx) <= 1 && std::abs(ay - by) <= 1;
}

auto main() -> int {
  Trie trie;
  trie.insert("abc");
  trie.insert("abd");
  trie.insert("dghkl");
  trie.insert("ab");
  trie.insert("lkjlksj");

  std::vector<std::string> grid_str;
  std::string line;
  std::size_t max_length = 0;
  while (std::cin >> line) {
    grid_str.push_back(line);
    max_length = std::max(max_length, line.size());
  }
  for (auto &l : grid_str) {
    std::size_t pad_len = max_length - l.size();
    for (std::size_t i = 0; i < pad_len; ++i) {
      l += '.';
    }
  }
  if (max_length == 0 || grid_str.empty()) {
    return EXIT_SUCCESS;
  }

  std::vector<std::vector<char>> grid_vec;
  grid_vec.reserve(grid_str.size());
  for (const auto &grid_row : grid_str) {
    std::cout << grid_row << std::endl;
    grid_vec.emplace_back(grid_row.begin(), grid_row.end());
  }

  Grid grid(grid_vec);
  Solver solver(grid, trie);
  auto word_paths = solver.find_all_words();

  for (const Solver::WordPath &path : word_paths) {
    for (std::size_t i = 0; i < path.size() - 1; ++i) {
      assert(adjacent(path[i].second, path[i + 1].second));  // NOLINT
    }
    std::set<Grid::Pos> visited_pos;
    for (const auto &[c, pos] : path) {
      assert(grid[pos] == c);  // NOLINT
      visited_pos.insert(pos);
    }
    assert(visited_pos.size() == path.size());  // NOLINT
  }
  std::cout << "Success: found " << word_paths.size() << " words" << std::endl;
  return EXIT_SUCCESS;
}
