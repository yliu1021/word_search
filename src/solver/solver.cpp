//
// Created by Yuhan Liu on 10/28/21.
//

#include "solver.h"

Solver::Solver(Grid g, Trie t) : grid_(std::move(g)), trie_(std::move(t)) {}

auto Solver::find_all_words() const noexcept -> std::vector<WordPath> {
  std::vector<WordPath> found_words;
  for (std::size_t row = 0; row < grid_.num_rows(); ++row) {
    for (std::size_t col = 0; col < grid_.num_cols(); ++col) {
      char c = grid_.at(row, col);
      if (std::islower(c) == 0) {
        continue;
      }
      Solver::WordPath path = {{c, {row, col}}};
      std::set<Grid::Pos> visited_pos;
      dfs_find_words(found_words, {row, col}, path, visited_pos,
                     trie_.root_node()->at(c));
    }
  }
  return found_words;
}

auto Solver::call_for_each_neighbor(
    const Grid::Pos& curr_pos,
    const std::function<void(const Grid::Pos&, char)>& fn) const noexcept
    -> void {
  const auto& [row, col] = curr_pos;
  std::vector<int> valid_d_rows = {0};
  if (row > 0) {
    valid_d_rows.push_back(-1);
  }
  if (row < grid_.num_rows() - 1) {
    valid_d_rows.push_back(1);
  }
  std::vector<int> valid_d_cols = {0};
  if (col > 0) {
    valid_d_cols.push_back(-1);
  }
  if (col < grid_.num_cols() - 1) {
    valid_d_cols.push_back(1);
  }
  for (int d_row : valid_d_rows) {
    for (int d_col : valid_d_cols) {
      if (d_row == 0 && d_col == 0) {
        continue;
      }
      Grid::Pos new_pos{row + d_row, col + d_col};
      fn(new_pos, grid_[new_pos]);
    }
  }
}

auto Solver::dfs_find_words(
    std::vector<WordPath>& acc, const Grid::Pos& pos, Solver::WordPath& path,
    std::set<Grid::Pos>& visited_pos,
    const std::shared_ptr<Trie::Node>& trie) const noexcept -> void {
  if (!trie) {
    return;
  }
  if (trie->is_leaf) {
    acc.push_back(path);
  }
  auto insert_loc = visited_pos.emplace(pos);
  call_for_each_neighbor(pos, [&](const Grid::Pos& new_pos, char new_c) {
    if (visited_pos.contains(new_pos)) {
      return;
    }
    if (std::islower(new_c) == 0) {
      return;
    }
    path.push_back({new_c, new_pos});
    dfs_find_words(acc, new_pos, path, visited_pos, trie->at(new_c));
    path.pop_back();
  });
  visited_pos.erase(insert_loc.first);
}
