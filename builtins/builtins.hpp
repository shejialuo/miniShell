#pragma once

#include <string>
#include <vector>

class Builtins {
public:
  Builtins() = delete;
  Builtins(const Builtins &) = delete;
  Builtins(Builtins &&) = delete;
  Builtins &operator=(const Builtins &) = delete;
  Builtins &operator=(Builtins &&) = delete;

  static bool cdHandler(std::vector<std::string> &arguments);
  static bool pwdHandler(std::vector<std::string> &arguments);
  static bool echoHandler(std::vector<std::string> &arguments);
  static bool exitHandler(std::vector<std::string> &arguments);
};
