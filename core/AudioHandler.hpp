#pragma once // AUDIO_HANDLER_HPP

#include "note.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <iostream>
#include <vector>

struct Chord {
  std::vector<Note> notes;

  Chord(Note *notes, int size) {
    for (int i = 0; i < size; i++) {
      this->notes.push_back(notes[i]);
    }
  }

  void append_to_vector(std::vector<Note> &v) {
    for (auto note : notes) {
      v.push_back(note);
    }
  }
};

struct AudioController {
  bool playing = false;
  SDL_AudioDeviceID dev = 0;

  void toggle() {
    std::cout << (playing ? "Pausing audio..." : "Playing audio...")
              << std::endl;
    playing = !playing;
    SDL_PauseAudioDevice(dev, (int)!playing);
  }
};

struct AudioData {
  std::vector<Note> active_notes;
};

class AudioHandler {
private:
  static void audio_callback(void *user_data, Uint8 *stream, int len) {
    AudioData *audio = static_cast<AudioData *>(user_data);
    Sint16 *buffer = reinterpret_cast<Sint16 *>(stream);
    int sample_count = len / sizeof(Sint16);

    const double sample_rate = 44100.0;
    const double amplitude = 20000.0;

    for (int i = 0; i < sample_count; ++i) {
      double mixed_sample = 0.0;

      if (!audio->active_notes.empty()) {
        for (auto &note : audio->active_notes) {
          mixed_sample += std::sin(note.phase);

          double coef = 1.0, dx = 1.0;
          int harmonics = 8;
          double wave = 0;
          for (int h = 0; h < harmonics; h++) {
            wave += dx * std::sin(coef * note.phase);
            dx /= 2;
            coef++;
          }

          mixed_sample += (wave / 1.875);

          note.phase += (2 * M_PI * note.frequency) / sample_rate;
          if (note.phase >= 2 * M_PI)
            note.phase -= 2 * M_PI;
        }

        double volume_scale =
            amplitude / std::max(1.0, (double)audio->active_notes.size());
        buffer[i] = static_cast<Sint16>(mixed_sample * volume_scale);
      } else
        buffer[i] = 0;
    }
  }

public:
  AudioHandler(int *result) {
    wanted_spec.freq = 44100.0;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = 1;
    wanted_spec.samples = 512;
    wanted_spec.callback = audio_callback;
    wanted_spec.userdata = &audio_data;

    ac.dev = SDL_OpenAudioDevice(nullptr, 0, &wanted_spec, nullptr, 0);

    if (ac.dev == 0) {
      std::cerr << "[ERROR] Failed to open audio device: " << SDL_GetError()
                << std::endl;
      *result = 1;
    }
    *result = 0;
  }

  ~AudioHandler() { SDL_CloseAudioDevice(ac.dev); }

private:
  SDL_AudioSpec wanted_spec{};

public:
  AudioController ac;
  AudioData audio_data;
};
