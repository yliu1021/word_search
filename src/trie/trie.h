//
// Created by Yuhan Liu on 10/27/21.
//

#ifndef WORD_SEARCH_SRC_TRIE_TRIE_H_
#define WORD_SEARCH_SRC_TRIE_TRIE_H_

#include <array>
#include <string>

enum class SearchResult { kFound, kPrefix, kNonexistent };

class Trie {
  static constexpr std::size_t num_buckets = 26;

 public:
  struct Node {
    void insert(const std::string &val, std::size_t pos) noexcept;
    [[nodiscard]] auto find(const std::string &val,
                            std::size_t pos) const noexcept -> SearchResult;
    [[nodiscard]] auto size() const noexcept -> std::size_t;
    [[nodiscard]] auto empty() const noexcept -> bool;

    [[nodiscard]] auto at(char c) const noexcept -> std::shared_ptr<Node>;
    auto operator[](char c) const noexcept -> std::shared_ptr<Node>;

    bool is_leaf = false;
    std::array<std::shared_ptr<Node>, num_buckets> children;
  };

  Trie();

  auto insert(const std::string &val) noexcept -> bool;
  [[nodiscard]] auto find(const std::string &val) const noexcept
      -> SearchResult;
  [[nodiscard]] auto size() const noexcept -> std::size_t;
  [[nodiscard]] auto empty() const noexcept -> bool;

  [[nodiscard]] auto root_node() const noexcept -> std::shared_ptr<Node>;

 private:
  std::shared_ptr<Node> root_node_;
};

#endif  // WORD_SEARCH_SRC_TRIE_TRIE_H_
