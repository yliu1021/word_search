#include <chrono>
#include <fstream>
#include <iostream>
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

  std::string line;
  std::vector<std::vector<char>> grid_vec;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      try {
        Grid g(grid_vec);
        Solver s(g, *corpus);
        auto all_words = s.find_all_words();
        std::sort(
            all_words.begin(), all_words.end(),
            [](const auto &a, const auto &b) { return a.size() < b.size(); });
        for (const Solver::WordPath &path : all_words) {
          for (const auto &[c, pos] : path) {
            std::cout << c << ':' << pos.first << ',' << pos.second << ' ';
          }
          std::cout << '\n';
        }
        std::cout << '-' << std::endl;
      } catch (const InvalidGrid &) {
        std::cout << "invalid grid" << std::endl;
      }
      grid_vec.clear();
    } else {
      grid_vec.emplace_back(line.begin(), line.end());
    }
  }

  return EXIT_SUCCESS;
}
