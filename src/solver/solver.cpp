//
// Created by Yuhan Liu on 10/28/21.
//

#include "solver.h"

Solver::Solver(Grid g, Trie t) : grid_(std::move(g)), trie_(std::move(t)) {}

auto Solver::find_all_words() const noexcept -> std::vector<WordPath> {
  std::vector<std::vector<bool>> visited_pos(
      grid_.num_rows(), std::vector<bool>(grid_.num_cols(), false));
  std::vector<WordPath> found_words;
  for (std::size_t row = 0; row < grid_.num_rows(); ++row) {
    for (std::size_t col = 0; col < grid_.num_cols(); ++col) {
      char c = grid_.at(row, col);
      if (std::islower(c) == 0) {
        continue;
      }
      Solver::WordPath path = {{c, {row, col}}};
      for (auto& visited_pos_row : visited_pos) {
        std::fill(visited_pos_row.begin(), visited_pos_row.end(), false);
      }
      dfs_find_words(found_words, {row, col}, path, visited_pos,
                     trie_.root_node()->at(c));
    }
  }
  return found_words;
}

auto Solver::dfs_find_words(
    std::vector<WordPath>& acc, const Grid::Pos& pos, Solver::WordPath& path,
    std::vector<std::vector<bool>>& visited_pos,
    const std::shared_ptr<Trie::Node>& trie) const noexcept -> void {
  if (!trie) {
    return;
  }
  if (trie->is_leaf) {
    acc.push_back(path);
  }
  visited_pos[pos.first][pos.second] = true;
  grid_.call_for_each_neighbor(pos, [&](const Grid::Pos& new_pos, char new_c) {
    if (visited_pos[new_pos.first][new_pos.second]) {
      return;
    }
    if (std::islower(new_c) == 0) {
      return;
    }
    path.push_back({new_c, new_pos});
    dfs_find_words(acc, new_pos, path, visited_pos, trie->at(new_c));
    path.pop_back();
  });
  visited_pos[pos.first][pos.second] = false;
}
