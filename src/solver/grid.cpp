//
// Created by Yuhan Liu on 10/29/21.
//

#include "grid.h"

#include <algorithm>
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
  const auto& [row, col] = pos;
  return this->at(row, col);
}

auto Grid::operator[](const std::pair<std::size_t, std::size_t>& pos) noexcept
    -> char& {
  const auto [row, col] = pos;
  return this->at(row, col);
}

auto Grid::num_rows() const noexcept -> std::size_t { return grid_.size(); }

auto Grid::num_cols() const noexcept -> std::size_t { return grid_[0].size(); }
