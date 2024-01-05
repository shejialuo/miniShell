#include "parser/parser.hpp"

#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>

void printUsage() { std::cout << "Usage: ./miniShell [file]\n"; }

void start(std::istream &is, bool isFile = false) {
  Parser parser{};
  while (true) {
    if (!isFile) {
      std::cout << "$ ";
      std::cout.flush();
    }
    std::string script{};
    std::getline(is, script);

    if (is.eof()) {
      return;
    }

    if (!script.empty() && script.back() == '\n') {
      script.erase(script.length() - 1);
    }

    if (script.empty()) {
      continue;
    }

    auto command = parser.parseScript(script);
    if (command != nullptr) {
      command->execute();
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 1 && argc != 2) {
    printUsage();
  }

  if (argc == 1) {
    start(std::cin);
  } else {
    std::ifstream ifs{argv[1]};
    if (!ifs.is_open()) {
      std::cout << "Cannot open file " << argv[1] << '\n';
      exit(1);
    }
    start(ifs, true);
  }

  return 0;
}
