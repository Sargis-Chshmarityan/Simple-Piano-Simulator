#include "piano.hpp"
#include <SDL2/SDL.h>

int main() {
  Piano piano;
  piano.play(1);
  piano.play(5);
  piano.play(8);
  return 0;
}
