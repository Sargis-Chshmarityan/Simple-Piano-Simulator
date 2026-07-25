#pragma once // WINDOW_HANDLER_H2P
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
    Piano piano(2);
    running = true;
    handler.ac.toggle();

    while (running) {
      while (SDL_PollEvent(&event)) {
        int key_root = piano.get_octave() * 12;
        if (event.type == SDL_QUIT) {
          running = false;
        }
        if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
          switch (event.key.keysym.sym) {
          case SDLK_a:
            handler.append_note(piano.get_key(key_root + 1));
            break;
          case SDLK_w:
            handler.append_note(piano.get_key(key_root + 2));
            break;
          case SDLK_s:
            handler.append_note(piano.get_key(key_root + 3));
            break;
          case SDLK_d:
            handler.append_note(piano.get_key(key_root + 4));
            break;
          case SDLK_r:
            handler.append_note(piano.get_key(key_root + 5));
            break;
          case SDLK_f:
            handler.append_note(piano.get_key(key_root + 6));
            break;
          case SDLK_t:
            handler.append_note(piano.get_key(key_root + 7));
            break;
          case SDLK_g:
            handler.append_note(piano.get_key(key_root + 8));
            break;
          case SDLK_h:
            handler.append_note(piano.get_key(key_root + 9));
            break;
          case SDLK_u:
            handler.append_note(piano.get_key(key_root + 10));
            break;
          case SDLK_j:
            handler.append_note(piano.get_key(key_root + 11));
            break;
          case SDLK_i:
            handler.append_note(piano.get_key(key_root + 12));
            break;
          case SDLK_ESCAPE:
            std::cout << "Escape pressed, exitting the main loop\n";
            running = false;
            break;
          case SDLK_KP_PLUS:
            piano.set_octave(piano.get_octave() + 1);
            std::cout << "Increasing octave. Current: " << piano.get_octave()
                      << std::endl;
            break;
          case SDLK_KP_MINUS:
            piano.set_octave(piano.get_octave() - 1);
            std::cout << "Decreasing octave. Current: " << piano.get_octave()
                      << std::endl;

            break;
          }
        }
        if (event.type == SDL_KEYUP && event.key.repeat == 0) {
          switch (event.key.keysym.sym) {
          case SDLK_a:
            handler.remove_note(key_root + 1);
            break;
          case SDLK_w:
            handler.remove_note(key_root + 2);
            break;
          case SDLK_s:
            handler.remove_note(key_root + 3);
            break;
          case SDLK_d:
            handler.remove_note(key_root + 4);
            break;
          case SDLK_r:
            handler.remove_note(key_root + 5);
            break;
          case SDLK_f:
            handler.remove_note(key_root + 6);
            break;
          case SDLK_t:
            handler.remove_note(key_root + 7);
            break;
          case SDLK_g:
            handler.remove_note(key_root + 8);
            break;
          case SDLK_h:
            handler.remove_note(key_root + 9);
            break;
          case SDLK_u:
            handler.remove_note(key_root + 10);
            break;
          case SDLK_j:
            handler.remove_note(key_root + 11);
            break;
          case SDLK_i:
            handler.remove_note(key_root + 12);
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
