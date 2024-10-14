#ifndef WINDOW_H_SENTRY
#define WINDOW_H_SENTRY

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "def.h"
#include "config.h"

extern cn_bool glb_running;
extern SDL_Window* glb_window;
extern SDL_Renderer* glb_renderer;

int init_window();
void close_window();

#endif
