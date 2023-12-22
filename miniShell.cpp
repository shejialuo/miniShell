#include <iostream>

void printUsage() { std::cout << "Usage: ./miniShell [file]\n"; }

int main(int argc, char *argv[]) {
  if (argc != 1 && argc != 2) {
    printUsage();
  }

  if (argc == 1) {
    std::cout << "This is a mini shell\n";
  } else {
    std::cout << "This is a mini shell called with a file " << argv[1] << '\n';
  }

  return 0;
}
