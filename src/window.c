#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include "config.h"
#include "error.h"
#include "def.h" 
#include "log.h"

cn_bool glb_running = cn_false;
SDL_Window* glb_window = NULL;
SDL_Renderer* glb_renderer = NULL;

static cn_bool is_init = cn_false;

static int create_window()
{
	uint32_t win_flags, render_flags;
	
	win_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	render_flags = SDL_RENDERER_ACCELERATED;
	
	glb_window = SDL_CreateWindow("Cnovel", SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED, 1024,
								  792, win_flags);
	if(!glb_window)
		return -1;
	glb_renderer = SDL_CreateRenderer(glb_window, -1, render_flags);
	if(!glb_renderer)
		return -1;
	SDL_SetRenderDrawColor(glb_renderer, 0, 0, 0, 255);
	SDL_RenderClear(glb_renderer);
	SDL_RenderPresent(glb_renderer);
	return 0;

}
int load_texture(const char* path, SDL_Texture* texture)
{
	SDL_Surface* loaded_surface;
	loaded_surface = IMG_Load(path);
	if(!loaded_surface) {
		printf_error(SDL_GetError());
		return -1;
	}
	texture = SDL_CreateTextureFromSurface(glb_renderer, loaded_surface);
	if(!texture) {
		printf_error(SDL_GetError());
		return -1;
	}
	printf_log("Load texture: %s", path);
	return 0;
}
int init_window()
{
	int st;
	if(is_init) {
		printf_error("Window subsystem has already been initialized");
		return -1;
	}
	st = SDL_Init(SDL_INIT_VIDEO /* | SDL_INIT_IMAGE */);
	if(st == -1) {
		fatal_error();
		printf_error(SDL_GetError());
		return -1;
	}
	st = create_window();
	if(st == -1) {
		fatal_error();
		printf_error(SDL_GetError());
		return -1;
	}
	glb_running = cn_true;
	is_init = cn_true;
	printf_log("Init window");
	return 0;
}

void close_window()
{
	printf_log("Close window");
	SDL_DestroyWindow(glb_window);
	SDL_DestroyRenderer(glb_renderer);
	SDL_Quit();	
}
