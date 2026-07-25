#pragma once // PIANO_HPP

#include "note.hpp"
#include <algorithm>
#include <vector>

typedef Note Key;

class Piano {
public:
  Piano(int initial_octave) {
    this->current_octave = initial_octave;
    for (int i = 0; i < KEY_COUNT; i++) {
      this->keys.push_back(
          Note{{note_names[i % 12], i / 12}, note_freq_calc(i + 1), 0, i + 1});
    }
  }

  void print_keys() {
    for (int i = 0; i < KEY_COUNT; i++)
      this->keys[i].print();
  }

  void play(int key_id) const { this->keys[key_id - 1].play(); }

  Key get_key(int index) const { return keys[index - 1]; }

  int get_octave() const { return this->current_octave; }

  void set_octave(int value) {
    value = std::clamp(value, 0, 6);
    this->current_octave = value;
  }

  ~Piano() {}

private:
  const int KEY_COUNT = 84;
  int current_octave = 0;
  std::vector<Key> keys;
  std::string note_names[12] = {"A",  "A#", "B", "C",  "C#", "D",
                                "D#", "E",  "F", "F#", "G",  "G#"};
};
