#pragma once // AUDIO_HANDLER_HPP

#include "note.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <numbers>

static constexpr double PI = std::numbers::pi;

struct Chord
{
  std::vector<Note> notes;

  Chord(Note *notes, int size)
  {
    for (int i = 0; i < size; i++)
    {
      this->notes.push_back(notes[i]);
    }
  }

  void append_to_vector(std::vector<Note> &v)
  {
    for (auto note : notes)
    {
      v.push_back(note);
    }
  }
};

struct AudioController
{
  bool playing = false;
  SDL_AudioDeviceID dev = 0;

  void toggle()
  {
    std::cout << (playing ? "Pausing audio..." : "Playing audio...")
              << std::endl;
    playing = !playing;
    SDL_PauseAudioDevice(dev, (int)!playing);
  }
};

struct AudioData
{
  std::vector<Note> active_notes;
};

class AudioHandler
{
private:
  static void audio_callback(void *user_data, Uint8 *stream, int len)
  {
    AudioData *audio = static_cast<AudioData *>(user_data);
    Sint16 *buffer = reinterpret_cast<Sint16 *>(stream);
    int sample_count = len / sizeof(Sint16);

    const double sample_rate = 44100.0;
    const double amplitude = 10000.0;

    for (int i = 0; i < sample_count; ++i)
    {
      double mixed_sample = 0.0;

      if (!audio->active_notes.empty())
      {
        for (auto &note : audio->active_notes)
        {
          note.age += 1.0 / sample_rate;
          mixed_sample += std::sin(note.phase);

          double coef = 1.0, dx = 1.0, total_weight = 1.0, decay_coef = -1.25;
          int harmonics = 8;
          std::vector<double> harmonics_decays;
          double wave = 0;
          for (int j = 0; j < harmonics; j++)
          {
            harmonics_decays.push_back(std::exp(decay_coef * note.age));
            decay_coef *= 2;
          }
          for (int h = 0; h < harmonics; h++)
          {
            double decay = harmonics_decays[h];
            wave += (dx * decay) * std::sin(coef * note.phase);
            dx /= 2;
            total_weight += (dx * decay);
            coef++;
          }

          mixed_sample += (wave / total_weight) * note.release_volume;

          if (note.is_releasing)
          {
            double release_step = 1.0 / (note.release_duration * sample_rate);
            note.release_volume -= release_step;
            if (note.release_volume < 0.0)
            {
              note.release_volume = 0.0;
            }
          }

          note.phase += (2 * PI * note.frequency) / sample_rate;
          if (note.phase >= 2 * PI)
            note.phase -= 2 * PI;

          std::erase_if(audio->active_notes, [](const Note &note)
                        { return note.is_releasing && note.release_volume <= 0.0; });
        }

        double active_count = static_cast<double>(audio->active_notes.size());
        double volume_scale =
            amplitude / std::sqrt(std::max(1.0, active_count));

        double final_val = mixed_sample * volume_scale;
        buffer[i] =
            static_cast<Sint16>(std::clamp(final_val, -32767.0, 32767.0));
      }
      else
        buffer[i] = 0;
    }
  }

public:
  AudioHandler(int *result)
  {
    wanted_spec.freq = 44100.0;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = 1;
    wanted_spec.samples = 128;
    wanted_spec.callback = audio_callback;
    wanted_spec.userdata = &audio_data;

    ac.dev = SDL_OpenAudioDevice(nullptr, 0, &wanted_spec, nullptr, 0);

    if (ac.dev == 0)
    {
      std::cerr << "[ERROR] Failed to open audio device: " << SDL_GetError()
                << std::endl;
      *result = 1;
    }
    *result = 0;
  }

  ~AudioHandler() { SDL_CloseAudioDevice(ac.dev); }

  int append_note(Note note)
  {
    int index = audio_data.active_notes.size();
    SDL_LockAudioDevice(ac.dev);
    audio_data.active_notes.push_back(note);
    SDL_UnlockAudioDevice(ac.dev);
    return index;
  }

  int remove_note(int index)
  {
    int id = 0;
    for (size_t i = 0; i < audio_data.active_notes.size(); i++)
    {
      if (index == audio_data.active_notes[i].index)
        id = i;
    }

    SDL_LockAudioDevice(ac.dev);
    audio_data.active_notes[id].is_releasing = true;
    SDL_UnlockAudioDevice(ac.dev);

    return 0;
  }

private:
  SDL_AudioSpec wanted_spec{};

public:
  AudioController ac;
  AudioData audio_data;
};
