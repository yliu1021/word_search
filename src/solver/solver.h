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
  auto call_for_each_neighbor(
      const Grid::Pos &curr_pos,
      const std::function<void(const Grid::Pos &, char c)> &fn) const noexcept
      -> void;
  auto dfs_find_words(std::vector<WordPath> &acc, const Grid::Pos &pos,
                      WordPath &path, std::set<Grid::Pos> &visited_pos,
                      const std::shared_ptr<Trie::Node> &trie) const noexcept
      -> void;

  Grid grid_;
  Trie trie_;
};

#endif  // WORD_SEARCH_SRC_SOLVER_SOLVER_H_
