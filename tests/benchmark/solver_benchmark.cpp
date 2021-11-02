//
// Created by Yuhan Liu on 10/31/21.
//

#include <chrono>
#include <fstream>
#include <iostream>
#include <optional>
#include <random>
#include <span>

#include "grid.h"
#include "solver.h"
#include "trie.h"

constexpr double milli_in_sec = 1000.0;

auto parse_corpus(const std::string &path) -> std::optional<Trie> {
  std::cerr << "Reading " << path << std::endl;
  std::ifstream stream(path, std::ios::in);
  if (!stream.is_open()) {
    std::cerr << "Unable to open file" << std::endl;
    stream.close();
    return std::nullopt;
  }
  std::size_t num_lines = 0;
  Trie trie;
  std::string line;
  auto start = std::chrono::high_resolution_clock::now();
  while (stream >> line) {
    bool success = trie.insert(line);
    if (!success) {
      std::cerr << "Unable to insert \"" << line << "\" into trie (line no. "
                << num_lines << ")... ignoring" << std::endl;
    }
    ++num_lines;
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  auto seconds = static_cast<double>(duration.count()) / milli_in_sec;
  stream.close();
  std::cerr << "Read " << num_lines << " lines in " << seconds << " secs"
            << std::endl;
  return trie;
}

auto benchmark_grid_size(std::size_t num_rows, std::size_t num_cols,
                         const Trie &corpus) {
  std::cerr << "Creating " << num_rows << " x " << num_cols << " grid... ";
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution('a', 'z');
  std::vector<std::vector<char>> grid_vec(num_rows,
                                          std::vector<char>(num_cols, '.'));
  for (auto &row : grid_vec) {
    for (auto &c : row) {
      c = static_cast<char>(distribution(generator));
    }
  }
  std::cerr << "Done" << std::endl;

  std::cerr << "Starting solve... ";
  Grid grid(grid_vec);
  Solver solver(grid, corpus);
  double tot_duration = 0;
  constexpr int num_runs = 10;
  std::vector<Solver::WordPath> word_paths;
  for (int i = 0; i < num_runs; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    word_paths = solver.find_all_words();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    auto seconds = static_cast<double>(duration.count()) / milli_in_sec;
    tot_duration += seconds;
  }
  double seconds = tot_duration / num_runs;
  std::cerr << "Done" << std::endl;
  auto kwords_per_second =
      static_cast<double>(word_paths.size()) / 1000.0 / seconds;  // NOLINT
  std::cerr << "Found " << word_paths.size() << " words in " << seconds
            << " seconds (" << kwords_per_second << " kwords/sec)" << std::endl;
}

auto main(int argc, const char *argv[]) -> int {
  auto args = std::span(argv, size_t(argc));
  if (argc != 2) {
    std::cerr << "Usage: " << args[0] << " [CORPUS_FILE]" << std::endl;
    return EXIT_FAILURE;
  }

  std::optional<Trie> corpus = parse_corpus(args[1]);
  if (corpus == std::nullopt) {
    std::cerr << "Couldn't parse corpus trie" << std::endl;
    return EXIT_FAILURE;
  }
  std::cerr << "Loaded corpus" << std::endl;

  benchmark_grid_size(10, 10, *corpus);     // NOLINT
  benchmark_grid_size(100, 10, *corpus);    // NOLINT
  benchmark_grid_size(100, 100, *corpus);   // NOLINT
  benchmark_grid_size(1000, 100, *corpus);  // NOLINT

  return EXIT_SUCCESS;
}
