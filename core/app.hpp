#pragma once // WINDOW_HANDLER_HPP
#include "audio_handler.hpp"
#include "piano.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_video.h>

class App {
public:
  int run() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
      std::cerr << "[ERROR] SDL initialization error: " << SDL_GetError()
                << std::endl;
      return 1;
    }

    SDL_Window *window =
        SDL_CreateWindow("Piano", SDL_WINDOWPOS_CENTERED,
                         SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if (!window) {
      std::cerr << "[ERROR] Failed to create SDL window: " << SDL_GetError()
                << std::endl;
      SDL_Quit();
      return 1;
    }

    int r = 0;
    AudioHandler handler(&r);
    if (r == 1) {
      SDL_Quit();
      return 1;
    }
    Piano piano(0);
    Note chord[3] = {piano.get_key(25), piano.get_key(28), piano.get_key(34)};
    Chord c(chord, 3);
    c.append_to_vector(handler.audio_data.active_notes);
    running = true;

    while (running) {
      while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
          running = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
          switch (event.key.keysym.sym) {
          case SDLK_SPACE:
            handler.ac.toggle();
            break;
          case SDLK_ESCAPE:
            std::cout << "Escape pressed, exitting the main loop\n";
            running = false;
            break;
          }
        }
      }
    }

    SDL_Quit();
    return 0;
  }

private:
  bool running = false;
  SDL_Event event;
};
