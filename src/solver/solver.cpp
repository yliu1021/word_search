//
// Created by Yuhan Liu on 10/28/21.
//

#include "solver.h"

#include <future>

Solver::Solver(Grid g, Trie t) : grid_(std::move(g)), trie_(std::move(t)) {}

auto Solver::find_all_words() const noexcept -> std::vector<WordPath> {
  constexpr std::size_t load_per_thread = 1;
  std::size_t rows_per_thread =
      (load_per_thread + grid_.num_cols() - 1) / grid_.num_cols();
  std::vector<std::future<std::vector<WordPath>>> workers;
  std::size_t row_start = 0;
  while (row_start < grid_.num_rows()) {
    std::size_t row_end =
        std::min(row_start + rows_per_thread, grid_.num_rows());
    workers.emplace_back(
        std::async(&Solver::single_thread_solve, this, row_start, row_end));
    row_start = row_end;
  }
  std::vector<WordPath> results;
  for (auto& worker : workers) {
    const auto& worker_res = worker.get();
    results.insert(results.end(), worker_res.begin(), worker_res.end());
  }
  return results;
}

auto Solver::single_thread_solve(std::size_t row_start,
                                 std::size_t row_end) const noexcept
    -> std::vector<WordPath> {
  std::vector<std::vector<bool>> visited_pos(
      grid_.num_rows(), std::vector<bool>(grid_.num_cols(), false));
  std::vector<WordPath> found_words;
  for (std::size_t row = row_start; row < row_end; ++row) {
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
  const auto& [row, col] = pos;
  visited_pos[row][col] = true;
  for (const auto& [d_row, d_col] : {std::pair<int, int>{-1, -1},
                                     {-1, 0},
                                     {-1, 1},
                                     {0, -1},
                                     {0, 1},
                                     {1, -1},
                                     {1, 0},
                                     {1, 1}}) {
    // for each neighbor...
    if (row == 0 && d_row == -1 || row == grid_.num_rows() - 1 && d_row == 1) {
      continue;
    }
    if (col == 0 && d_col == -1 || col == grid_.num_cols() - 1 && d_col == 1) {
      continue;
    }
    Grid::Pos new_pos = {row + d_row, col + d_col};
    char new_c = grid_[new_pos];
    if (visited_pos[new_pos.first][new_pos.second]) {
      continue;
    }
    if (std::islower(new_c) == 0) {
      continue;
    }
    path.push_back({new_c, new_pos});
    dfs_find_words(acc, new_pos, path, visited_pos, trie->at(new_c));
    path.pop_back();
  }
  visited_pos[row][col] = false;
}
