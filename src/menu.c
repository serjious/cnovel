#include "menu.h"
#include "window.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_render.h"

static SDL_Texture* menu_texture(const char* path)
{
    static SDL_Texture* texture = NULL;
    if(path)
        texture = load_texture(path);
    return texture;
}


void prepareScene()
{
    SDL_Renderer* renderer = get_renderer();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

void presentScene()
{
    SDL_Renderer* renderer = get_renderer();
    SDL_RenderPresent(renderer);
}

void blit()
{
    static int is_load = 0;
    SDL_Rect dest;
    SDL_Renderer* renderer = get_renderer();
    SDL_Texture* texture;
    if(!is_load) {
        texture = menu_texture("test/project.png");
        is_load = 1;
    }

    texture = menu_texture(NULL);
    dest.x = 100;
    dest.y = 100;

    SDL_QueryTexture(texture, NULL, NULL, &dest.x, &dest.y);
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}

