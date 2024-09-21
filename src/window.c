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
static cn_app main_window(SDL_Window* new_window, SDL_Renderer* new_renderer, variable_control st)
{
    static cn_app app = {NULL, NULL};
    if(st == cn_set) {
        app.window = new_window;
        app.renderer = new_renderer;
    }
    return app;
}

void set_window(SDL_Window* new_window)
{
    main_window(new_window, NULL, cn_set);
}
void set_renderer(SDL_Renderer* new_renderer)
{
    main_window(NULL, new_renderer, cn_set);
}

SDL_Window* get_window()
{
    cn_app app = main_window(NULL, NULL, cn_get);
    return app.window;
}
SDL_Renderer* get_renderer()
{
    cn_app app = main_window(NULL, NULL, cn_get);
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
        fatal_error(cn_fatal);
        send_message_error(SDL_GetError());
        return -1;
    }

    init = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    if(-1 == init) {
        fatal_error(cn_fatal);
        send_message_error(SDL_GetError());
        return -1;
    }

    init = create_window(cfg);
    if(-1 == init) {
        fatal_error(cn_fatal);
        send_message_error(SDL_GetError());
        return -1;
    }
	
	send_message_log("Initialization window");
	return 0;
}

void close_window()
{
    SDL_Window* window = get_window(NULL);
    SDL_DestroyWindow(window);
    SDL_Quit();
	send_message_log("Close window");
}

