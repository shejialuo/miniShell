#include "builtins.hpp"

#include <iostream>
#include <stdint.h>
#include <string>
#include <sys/stat.h>
#include <unistd.h>
#include <vector>

bool Builtins::cdHandler(std::vector<std::string> &arguments) {
  if (arguments.size() != 1) {
    std::cout << "shell: cd: too many arguments\n";
    return false;
  }

  if (chdir(arguments[0].c_str()) != 0) {
    std::cout << "chdir system call error\n";
    return false;
  }

  return true;
}

bool Builtins::echoHandler(std::vector<std::string> &arguments) {
  for (int i = 0; i < arguments.size(); i++) {
    std::cout << arguments[i];
    if (i != arguments.size() - 1) {
      std::cout << ' ';
    }
  }

  std::cout << '\n';
  return true;
}

bool Builtins::pwdHandler(std::vector<std::string> &arguments) {
  if (!arguments.empty()) {
    std::cout << "shell: pwd: too many arguments\n";
    return false;
  }

  constexpr int bufferSize = 1000;

  std::string workingDirectory{};
  workingDirectory.resize(bufferSize);

  if (getcwd(workingDirectory.data(), bufferSize) == nullptr) {
    std::cout << "shell: pwd: cannot get current working directory\n";
    return false;
  };

  std::cout << workingDirectory << '\n';
  return true;
}

bool Builtins::exitHandler(std::vector<std::string> &arguments) {
  if (arguments.size() > 1) {
    std::cout << "shell: exit: too many arguments\n";
    return false;
  }

  if (arguments.empty()) {
    exit(0);
  } else {
    try {
      uint8_t exitStatus = std::stoi(arguments[0]);
      exit(exitStatus);
    } catch (...) {
      std::cout << "shell: exit: cannot convert the string to num\n";
    }
  }

  return true;
}
