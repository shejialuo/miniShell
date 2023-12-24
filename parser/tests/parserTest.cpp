#include "parser.hpp"

#include <gtest/gtest.h>
#include <string>
#include <unordered_map>
#include <vector>

TEST(Parser, parserSuccess) {
  Parser parser{};

  std::unordered_map<std::string, std::string> success{
      {"a=3", "a=3"},
      {"echo a b c", "echo a b c"},
  };

  for (const auto &[input, expected] : success) {
    ASSERT_EQ(parser.parseScript(input)->inspect(), expected);
  }
}

TEST(Parser, parserFailed) {
  Parser parser{};

  std::vector<std::string> failed{
      "=", "a=", "=b", "command |", "| command", "| | |", "= = =",
  };

  for (auto &&f : failed) {
    ASSERT_EQ(parser.parseScript(f), nullptr);
  }
}