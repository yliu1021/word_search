//
// Created by Yuhan Liu on 10/29/21.
//

#include "grid.h"

#include <utility>

Grid::Grid(std::vector<std::vector<char>> grid) : grid_(std::move(grid)) {
  if (grid_.empty()) {
    throw InvalidGrid();
  }
  if (std::any_of(grid_.begin(), grid_.end(), [&](const auto& row) {
        return row.size() != grid_[0].size();
      })) {
    // check to make sure all the rows are the same length
    throw InvalidGrid();
  }
}

Grid::Grid(const Grid& other) = default;
Grid::Grid(Grid&& other) noexcept = default;
auto Grid::operator=(const Grid& other) -> Grid& = default;
auto Grid::operator=(Grid&& other) noexcept -> Grid& = default;
Grid::~Grid() = default;

auto Grid::at(std::size_t row, std::size_t col) const noexcept -> char {
  return grid_[row][col];
}

auto Grid::at(std::size_t row, std::size_t col) noexcept -> char& {
  return grid_[row][col];
}

auto Grid::operator[](
    const std::pair<std::size_t, std::size_t>& pos) const noexcept -> char {
  const auto [row, col] = pos;
  return this->at(row, col);
}

auto Grid::operator[](const std::pair<std::size_t, std::size_t>& pos) noexcept
    -> char& {
  const auto [row, col] = pos;
  return this->at(row, col);
}

auto Grid::num_rows() const noexcept -> std::size_t { return grid_.size(); }

auto Grid::num_cols() const noexcept -> std::size_t { return grid_[0].size(); }

auto Grid::call_for_each_neighbor(
    const Grid::Pos& curr_pos,
    const std::function<void(const Grid::Pos&, char)>& fn) const noexcept
    -> void {
  const auto& [row, col] = curr_pos;
  for (int d_row : {-1, 0, 1}) {
    for (int d_col : {-1, 0, 1}) {
      if (d_row == 0 && d_col == 0) {
        continue;
      }
      if (row == 0 && d_row == -1 || row == num_rows() - 1 && d_row == 1) {
        continue;
      }
      if (col == 0 && d_col == -1 || col == num_cols() - 1 && d_col == 1) {
        continue;
      }
      Pos new_pos = {row + d_row, col + d_col};
      fn(new_pos, (*this)[new_pos]);
    }
  }
}
