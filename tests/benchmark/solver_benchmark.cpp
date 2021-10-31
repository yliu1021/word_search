//
// Created by Yuhan Liu on 10/31/21.
//

#include <chrono>
#include <fstream>
#include <iostream>
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

  std::cerr << "Creating grid... ";
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<char> distribution('a', 'z');
  constexpr std::size_t num_rows = 100;
  constexpr std::size_t num_cols = 100;
  std::vector<std::vector<char>> grid_vec(num_rows,
                                          std::vector<char>(num_cols, '.'));
  for (auto &row : grid_vec) {
    for (auto &c : row) {
      c = distribution(generator);
    }
  }
  std::cerr << "Done" << std::endl;

  std::cerr << "Starting solve... ";
  Grid grid(grid_vec);
  Solver solver(grid, *corpus);
  auto start = std::chrono::high_resolution_clock::now();
  auto word_paths = solver.find_all_words();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  auto seconds = static_cast<double>(duration.count()) / milli_in_sec;
  std::cerr << "Done" << std::endl;
  std::cerr << "Found " << word_paths.size() << " words in " << seconds
            << " seconds" << std::endl;

  return EXIT_SUCCESS;
}
