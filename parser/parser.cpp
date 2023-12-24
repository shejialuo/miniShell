#include "parser.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

bool Parser::parseHelper(const std::string &script, int start, int end,
                         std::vector<std::unique_ptr<CommandBase>> &commands) {
  bool firstWord = true;

  std::string command{};
  std::vector<std::string> arguments{};

  std::optional<std::string> redirectFile{std::nullopt};
  RedirectStatus status = RedirectStatus::none;

  int wordStart = start;

  while (wordStart < script.size()) {
    // Skip the spaces when scanning a new word
    while (wordStart < script.size() && script[wordStart] == ' ') {
      wordStart++;
    }

    int wordEnd = wordStart;

    while (wordEnd < script.size() && script[wordEnd] != ' ' &&
           script[wordEnd] != '>' && script[wordEnd] != '<') {
      wordEnd++;
    }

    // There may be situations that we could encounter `echo "test" >
    // /tmp/tmp.txt`. and in this situation, we could have `wordStart == wordEnd
    // when parsing the ` >`.
    if (wordStart != wordEnd) {
      std::string sub = script.substr(wordStart, wordEnd - wordStart);
      if (firstWord) {
        command = std::move(sub);
        size_t index = command.find('=');
        if (index != command.npos) {
          // We find the variable here `a=3`.
          if (index == 0 || index == script.size() - 1) {
            std::cout << "[syntax error]: the assignment statement cannot be "
                         "parsed\n";
            return false;
          }

          commands.emplace_back(std::make_unique<Variable>(std::move(command)));
          return true;
        }
        firstWord = false;
      } else {
        if (status == RedirectStatus::none) {
          arguments.push_back(std::move(sub));
        } else {
          redirectFile = std::move(sub);
          return true;
        }
      }
    }

    if (script[wordEnd] == ' ') {
      wordEnd++;
    } else if (script[wordEnd] == '>') {
      if (firstWord) {
        std::cout << "[syntax error]: no command for redirection\n";
        return false;
      }

      if (wordEnd + 1 < script.size()) {
        status = RedirectStatus::truncate;
        wordEnd++;
        if (script[wordEnd + 1] == '>') {
          status = RedirectStatus::append;
          wordEnd++;
        }
      } else {
        std::cout << "[syntax error]: no file for redirection\n";
        return false;
      }

    } else if (script[wordEnd] == '<') {
      if (firstWord) {
        std::cout << "[syntax error]: no command for redirection\n";
        return false;
      }

      status = RedirectStatus::input;
      wordEnd++;
    }
    wordStart = wordEnd;
  }

  if (command.empty()) {
    std::cout << "[syntax error]: cannot parse " << script << '\n';
    return false;
  }

  commands.emplace_back(
      std::make_unique<Command>(std::move(command), std::move(arguments),
                                std::move(redirectFile), status));

  return true;
}

std::unique_ptr<CommandBase> Parser::parseScript(const std::string &script) {
  size_t start = 0, end = 0;
  std::vector<std::unique_ptr<CommandBase>> commands{};

  while (end < script.size()) {
    if (script[end] == '|') {
      if (end == 0 || !parseHelper(script, start, end, commands)) {
        return nullptr;
      }
      start = end + 1;
    }
    end++;
  }

  if (!parseHelper(script, start, end, commands)) {
    return nullptr;
  }

  if (commands.size() == 1) {
    return std::move(commands[0]);
  } else {
    return std::make_unique<PipeCommand>(std::move(commands));
  }
}
