#include "command.hpp"

#include <iostream>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

void Variable::execute() {}

void Command::execute() {
  pid_t pid = fork();
  if (pid == -1) {
    std::cout << "[fork error]: cannot fork\n";
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    std::vector<char *> argv{};
    argv.resize(arguments.size() + 2);

    argv[0] = const_cast<char *>(command.c_str());
    for (size_t i = 0; i < arguments.size(); i++) {
      argv[i + 1] = const_cast<char *>(arguments[i].c_str());
    }

    argv.back() = nullptr;

    execv(command.c_str(), argv.data());
  } else {
    wait(NULL);
  }
}

void PipeCommand::execute() {}
