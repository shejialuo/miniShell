#pragma once

#include "command/command.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

/**
 * @brief The `Parser` class will parse the shell script.
 *
 * @details We will handle the shell script for simplicity, there
 * is no loop or branch control, we only consider about the following
 * 1. `command1 | command2 | command3 | command4` -> `PipeCommand`
 * 2. `command1 > file` -> `Command`
 * 3. `command1 >> file` -> `Command`
 * 4. `command1 < file` -> `Command`
 * 5. `command1 &` -> `Command`.
 *
 */
class Parser {
private:
  /**
   * @brief Parse the script from [start, end) to generate the
   * `Command`.
   *
   * @param script the script
   * @param start the command start
   * @param end the command end (exclusive)
   * @param commands the commands we should insert
   *
   * @return whether the parse is OK
   */
  bool parseHelper(const std::string &script, int start, int end,
                   std::vector<std::unique_ptr<CommandBase>> &commands);

public:
  Parser() = default;

  Parser(const Parser &) = delete;
  Parser(Parser &&) = delete;
  Parser &operator=(const Parser &) = delete;
  Parser &operator=(Parser &&) = delete;

  /**
   * @brief Parse the script and return the command for later
   * handling
   *
   * @details
   * 1. For `command1 | command2 | command3` we would
   * return the `PipeCommand`.
   * 2. For `a=3`, we would return `Variable`.
   * 3. For `command 1 > (&)`, we would return `Command`.
   *
   * @param script the script line one by one.
   * @return CommandBase
   */
  std::unique_ptr<CommandBase> parseScript(const std::string &script);
};
