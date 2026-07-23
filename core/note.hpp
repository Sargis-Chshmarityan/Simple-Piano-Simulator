#pragma once // NOTE_H

#include <cmath>
#include <iostream>
#include <string>

typedef double freq_t;

inline double note_freq_calc(int key_id) {
  return 440 * std::pow(2, (key_id - 49) / 12.0);
}

struct NoteName {
  std::string name;
  int octave;

  NoteName(std::string name, int octave) {
    this->name = name;
    this->octave = octave;
  }

  std::string to_string() const {
    std::string result = name + std::to_string(octave);
    return result;
  }
};

struct Note {
  NoteName name;
  freq_t frequency;
  double phase;

  Note(NoteName name, freq_t frequency, double phase)
      : name(name), frequency(frequency), phase(phase) {}

  void play() const {
    std::cout << "Playing: " << name.to_string() << ". Frequency: " << frequency
              << std::endl;
  }

  void print() const {
    std::cout << "Name: " << name.to_string() << ", Frequency: " << frequency
              << std::endl;
  }
};
