#ifndef WINDOW_H_SENTRY
#define WINDOW_H_SENTRY

#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include "config.h"
/*mb retrun window.c*/
typedef struct m_app{
    SDL_Window* window;
    SDL_Renderer* renderer;
} cn_app;

SDL_Texture* load_texture(const char* path);

void set_window(SDL_Window* new_window);
void set_renderer(SDL_Renderer* new_renderer);

SDL_Window* get_window();
SDL_Renderer* get_renderer();

int init_window(config* cfg);

int run_window();

void close_window();

#endif
