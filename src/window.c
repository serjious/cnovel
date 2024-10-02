#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>
#include <stdint.h>
#include "error.h"
#include "window.h"
#include "def.h"
#include "sound.h"
#include "log.h"


SDL_Texture* load_texture(const char* path)
{
    SDL_Texture* texture;
    SDL_Renderer* renderer;

    renderer = get_renderer();
    texture = IMG_LoadTexture(renderer, path);
    return texture;
}


/*fix lenght*/
static cn_app app = {NULL, NULL};

void set_window(SDL_Window* new_window)
{
	app.window = new_window; 
}
void set_renderer(SDL_Renderer* new_renderer)
{
	app.renderer = new_renderer;  
}

SDL_Window* get_window()
{
    return app.window;
}
SDL_Renderer* get_renderer()
{
    return app.renderer;
}

static int create_window(config* cfg)
{
    uint32_t window_flags, renderer_flags;
    SDL_Window* window = get_window();
    SDL_Renderer* renderer = get_renderer();

    window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    if(cfg->fullscreen)
        window_flags |= SDL_WINDOW_FULLSCREEN;

    window = SDL_CreateWindow("Cnovel", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, cfg->width,
                              cfg->height, window_flags);
    if(!window)
        return -1;


    renderer_flags = SDL_RENDERER_ACCELERATED;
    renderer = SDL_CreateRenderer(window, -1, renderer_flags);
    if(!renderer)
        return -1;

    set_window(window);
    set_renderer(renderer);
    return 0;
}

int run_window()
{
    return 1;
}

int init_window(config* cfg)
{
	int init;

	init = SDL_Init(SDL_INIT_VIDEO);
    if(-1 == init) {
        fatal_error();
        printf_error(SDL_GetError());
        return -1;
    }
    init = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    if(-1 == init) {
        fatal_error();
        printf_error(SDL_GetError());
        return -1;
    }
    init = create_window(cfg);
    if(-1 == init) {
        fatal_error();
        printf_error(SDL_GetError());
        return -1;
    }
	printf_log("Initialization window");
	return 0;
}

void close_window()
{
    SDL_Window* window = get_window(NULL);
    SDL_DestroyWindow(window);
    SDL_Quit();
	printf_log("Close window");
}

