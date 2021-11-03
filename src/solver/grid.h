//
// Created by Yuhan Liu on 10/29/21.
//

#ifndef WORD_SEARCH_SRC_SOLVER_GRID_H_
#define WORD_SEARCH_SRC_SOLVER_GRID_H_

#include <functional>
#include <vector>

class InvalidGrid : std::exception {};

class Grid {
 public:
  explicit Grid(std::vector<std::vector<char>> grid);

  Grid(const Grid &other);
  Grid(Grid &&other) noexcept;
  auto operator=(const Grid &other) -> Grid &;
  auto operator=(Grid &&other) noexcept -> Grid &;
  ~Grid();

  [[nodiscard]] auto at(std::size_t row, std::size_t col) const noexcept
      -> char;
  auto at(std::size_t row, std::size_t col) noexcept -> char &;

  using Pos = std::pair<std::size_t, std::size_t>;
  auto operator[](const Pos &pos) const noexcept -> char;
  auto operator[](const Pos &pos) noexcept -> char &;

  [[nodiscard]] auto num_rows() const noexcept -> std::size_t;
  [[nodiscard]] auto num_cols() const noexcept -> std::size_t;

 private:
  std::vector<std::vector<char>> grid_;
};

#endif  // WORD_SEARCH_SRC_SOLVER_GRID_H_
