#pragma once // PIANO_H

#include "note.hpp"
#include <memory>
#include <vector>

typedef Note Key;

class Piano {
public:
  Piano() {
    for (int i = 0; i < KEY_COUNT; i++) {
      keys.push_back(std::make_unique<Key>(
          Note{{note_names[i % 12], i / 12}, note_freq_calc(i + 1)}));
    }
  }

  void print_keys() {
    for (int i = 0; i < KEY_COUNT; i++)
      keys[i]->print();
  }

  void play(int key_id) const { keys[key_id - 1]->play(); }

  ~Piano() {}

private:
  const int KEY_COUNT = 88;
  std::vector<std::unique_ptr<Key>> keys;
  std::string note_names[12] = {"A",  "A#", "B", "C",  "C#", "D",
                                "D#", "E",  "F", "F#", "G",  "G#"};
};
