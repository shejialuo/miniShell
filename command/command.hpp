#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * @brief Represent the redirect status
 *
 * @details
 * 1. truncate -> `>`
 * 2. append -> `>>`
 * 3. input -> `<`
 *
 */
enum class RedirectStatus {
  truncate,
  append,
  input,
  none,
};

/**
 * @brief The interface
 *
 */
class CommandBase {
public:
  /**
   * @brief Execute the command
   *
   */
  virtual void execute() = 0;
  virtual std::string inspect() = 0;
  virtual ~CommandBase() = default;
};

/**
 * @brief `Variable` is used to handle the script like the
 * following: `a=3`, `b=4`.
 *
 * @details For `a=3`, the command should be `a=3`, after
 * calling `execute`, identifier should be 3 and value should
 * be 3.
 *
 */
class Variable : public CommandBase {
private:
  std::string command;

public:
  Variable() = delete;
  Variable(std::string &&c) : CommandBase{}, command{std::move(c)} {}

  /**
   * @brief Parse the `a=3`, extract the identifier and the
   * value, for simplicity, we do not check.
   *
   */
  virtual void execute() final;

  virtual std::string inspect() final { return command; }

  virtual ~Variable() = default;
};

/**
 * @brief If there is no pipe, we should only have one command to
 * execute, we should indicate the command and the arguments.
 * If encountering the `>`, `<` or `>>`, we should record the status
 * and the filename.
 *
 */
class Command : public CommandBase {
private:
  std::string command;
  std::vector<std::string> arguments;
  std::optional<std::string> redirectFile;
  RedirectStatus redirectStatus;

public:
  static std::unordered_map<std::string, std::function<bool(std::vector<std::string> &)>> builtins;

  Command() = delete;
  Command(std::string &&c, std::vector<std::string> &&ar, std::optional<std::string> &&rf, RedirectStatus status)
      : CommandBase{}
      , command{std::move(c)}
      , arguments{std::move(ar)}
      , redirectFile{std::move(rf)}
      , redirectStatus{status} {}

  virtual void execute() final;
  virtual std::string inspect() final {
    std::string result = command;
    for (const auto &argument : arguments) {
      result += " " + argument;
    }
    return result;
  }
  virtual ~Command() = default;
};

/**
 * @brief `PipeCommand` is used to represent the commands.
 * Because there are relationship requirements for these
 * commands, we should override the `execute` method here.
 *
 */
class PipeCommand : public CommandBase {
private:
  std::vector<std::unique_ptr<CommandBase>> commands{};

public:
  PipeCommand() = delete;
  PipeCommand(std::vector<std::unique_ptr<CommandBase>> &&c) : commands{std::move(c)} {}

  virtual void execute() final;
  virtual std::string inspect() final {
    std::string result = "";
    for (const auto &command : commands) {
      result += command->inspect() + " | ";
    }
    return result;
  }

  virtual ~PipeCommand() = default;
};