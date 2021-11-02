//
// Created by Yuhan Liu on 10/27/21.
//

#include "trie.h"

#include <algorithm>

static auto char_to_index(char c) -> std::size_t { return c - 'a'; }

void Trie::Node::insert(const std::string& val, std::size_t pos) noexcept {
  if (val.size() == pos) {
    is_leaf = true;
    return;
  }
  auto ind = char_to_index(val[pos]);
  if (!children.at(ind)) {
    children.at(ind) = std::make_unique<Node>();
  }
  children.at(ind)->insert(val, pos + 1);
}

auto Trie::Node::find(const std::string& val, std::size_t pos) const noexcept
    -> SearchResult {
  if (val.size() == pos) {
    if (is_leaf) {
      return SearchResult::kFound;
    }
    return SearchResult::kPrefix;
  }
  auto ind = char_to_index(val[pos]);
  if (children.at(ind)) {
    return children.at(ind)->find(val, pos + 1);
  }
  return SearchResult::kNonexistent;
}

auto Trie::Node::size() const noexcept -> std::size_t {
  std::size_t size = 0;
  if (is_leaf) {
    size += 1;
  }
  for (const auto& c : children) {
    if (c) {
      size += c->size();
    }
  }
  return size;
}

auto Trie::Node::empty() const noexcept -> bool {
  if (is_leaf) {
    return false;
  }
  return std::all_of(children.begin(), children.end(),
                     [](const auto& c) { return c.get() == nullptr; });
}

auto Trie::Node::at(char c) const noexcept -> std::shared_ptr<Node> {
  return children.at(char_to_index(c));
}

auto Trie::Node::operator[](char c) const noexcept -> std::shared_ptr<Node> {
  return this->at(c);
}

Trie::Trie() : root_node_(new Node) {}

auto Trie::insert(const std::string& val) noexcept -> bool {
  for (char c : val) {
    if (c < 'a' || 'z' < c) {
      return false;
    }
  }
  root_node_->insert(val, 0);
  return true;
}

auto Trie::find(const std::string& val) const noexcept -> SearchResult {
  return root_node_->find(val, 0);
}

auto Trie::size() const noexcept -> std::size_t { return root_node_->size(); }

auto Trie::empty() const noexcept -> bool { return root_node_->empty(); }

auto Trie::root_node() const noexcept -> std::shared_ptr<Trie::Node> {
  return root_node_;
}
