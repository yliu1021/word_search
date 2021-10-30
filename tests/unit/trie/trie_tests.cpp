//
// Created by Yuhan Liu on 10/29/21.
//

#include <gtest/gtest.h>

#include "trie.h"

class TrieTests : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}

  auto GetTrie() -> Trie& { return trie_; }

 private:
  Trie trie_;
};

TEST_F(TrieTests, TestEmpty) {  // NOLINT
  EXPECT_TRUE(GetTrie().empty());
  EXPECT_EQ(GetTrie().size(), 0);
}

TEST_F(TrieTests, TestExists) {  // NOLINT
  GetTrie().insert("abc");
  EXPECT_EQ(GetTrie().find("abc"), SearchResult::kFound);
}

TEST_F(TrieTests, TestPrefix) {  // NOLINT
  GetTrie().insert("abc");
  EXPECT_EQ(GetTrie().find("ab"), SearchResult::kPrefix);
}

TEST_F(TrieTests, TestNotFound) {  // NOLINT
  GetTrie().insert("abc");
  EXPECT_EQ(GetTrie().find("x"), SearchResult::kNonexistent);
  EXPECT_EQ(GetTrie().find("abd"), SearchResult::kNonexistent);
}
