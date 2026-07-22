#pragma once // NOTE_H

#include <cmath>
#include <iostream>
#include <string>

typedef float freq_t;

inline float note_freq_calc(int key_id) {
  return 440 * std::pow(2, (key_id - 49) / 12.0f);
}

struct NoteName {
  std::string name;
  int octave;

  std::string to_string() const {
    std::string result = name + std::to_string(octave);
    return result;
  }
};

struct Note {
  NoteName name;
  freq_t frequency;

  void play() const {
    std::cout << "Playing: " << name.to_string() << ". Frequency: " << frequency
              << std::endl;
  }

  void print() const {
    std::cout << "Name: " << name.to_string() << ", Frequency: " << frequency
              << std::endl;
  }
};
