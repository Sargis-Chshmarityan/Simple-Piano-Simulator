#include "app.hpp"

void start(int argc, char *argv[]) {
  std::cout << "Running main with args:" << ((argc == 1) ? " - " : "");
  for (int i = 1; i < argc; i++) {
    std::cout << argv[i] << " ";
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  start(argc, argv);
  App app;
  return app.run();
}
