//
// Created by Yuhan Liu on 10/29/21.
//

#include <gtest/gtest.h>

#include <memory>

#include "grid.h"
#include "solver.h"
#include "trie.h"

class SolverTests : public ::testing::Test {
 protected:
  void SetUp() override {
    trie_.insert("abc");
    trie_.insert("axx");
    trie_.insert("accxxb");
    trie_.insert("abd");
    trie_.insert("abcd");
    std::vector<std::vector<char>> grid_vec{{'a', 'x', '.', '.'},
                                            {'b', 'c', 'x', '.'},
                                            {'.', 'c', '.', 'a'},
                                            {'.', '.', '.', '.'}};
    grid_ = std::make_shared<Grid>(grid_vec);
    solver_ = std::make_shared<Solver>(*grid_, trie_);
  }

  auto GetSolver() -> std::shared_ptr<Solver> { return solver_; }

 private:
  Trie trie_;
  std::shared_ptr<Grid> grid_;
  std::shared_ptr<Solver> solver_;
};

TEST_F(SolverTests, FindWords) {  // NOLINT
  auto word_paths = GetSolver()->find_all_words();
  auto contains = [&](const Solver::WordPath &test_path) {
    for (const auto &path : word_paths) {
      if (path.size() != test_path.size()) {
        continue;
      }
      bool equal = true;
      for (std::size_t i = 0; i < test_path.size(); ++i) {
        if (path[i] != test_path[i]) {
          equal = false;
          break;
        }
      }
      if (equal) {
        return true;
      }
    }
    return false;
  };
  ASSERT_EQ(word_paths.size(), 5);
  EXPECT_TRUE(contains({{'a', {0, 0}}, {'x', {0, 1}}, {'x', {1, 2}}}));
  EXPECT_TRUE(contains({{'a', {0, 0}}, {'b', {1, 0}}, {'c', {1, 1}}}));
  EXPECT_TRUE(contains({{'a', {0, 0}}, {'b', {1, 0}}, {'c', {2, 1}}}));
  EXPECT_TRUE(contains({{'a', {0, 0}},
                        {'c', {1, 1}},
                        {'c', {2, 1}},
                        {'x', {1, 2}},
                        {'x', {0, 1}},
                        {'b', {1, 0}}}));
  EXPECT_TRUE(contains({{'a', {2, 3}}, {'x', {1, 2}}, {'x', {0, 1}}}));
}
