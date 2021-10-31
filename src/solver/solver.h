//
// Created by Yuhan Liu on 10/28/21.
//

#ifndef WORD_SEARCH_SRC_SOLVER_SOLVER_H_
#define WORD_SEARCH_SRC_SOLVER_SOLVER_H_

#include <functional>
#include <set>
#include <string>
#include <vector>

#include "grid.h"
#include "trie.h"

class Solver {
 public:
  explicit Solver(Grid g, Trie t);

  using WordPath = std::vector<std::pair<char, Grid::Pos>>;
  [[nodiscard]] auto find_all_words() const noexcept -> std::vector<WordPath>;

 private:
  [[nodiscard]] auto single_thread_solve(std::size_t row_start,
                                         std::size_t row_end) const noexcept
      -> std::vector<WordPath>;

  auto dfs_find_words(std::vector<WordPath> &acc, const Grid::Pos &pos,
                      WordPath &path,
                      std::vector<std::vector<bool>> &visited_pos,
                      const std::shared_ptr<Trie::Node> &trie) const noexcept
      -> void;

  Grid grid_;
  Trie trie_;
};

#endif  // WORD_SEARCH_SRC_SOLVER_SOLVER_H_
