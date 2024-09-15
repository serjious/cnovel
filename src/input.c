#include "input.h"
#include "def.h"
#include "sound.h"
#include "save.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_joystick.h>

int key_handler()
{
    SDL_Event event;
    static cn_bool is_pause = cn_false;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_KEYDOWN) {
            switch(event.key.keysym.sym) {
			case SDLK_s:
				save_audio_to_file();	
				break;
            case SDLK_ESCAPE:
                return 1;
            case SDLK_q:
                return 1;
            case SDLK_SPACE:
                if(is_pause == cn_false) {
                    pause_music();
                    is_pause = cn_true;
                } else {
                    resume_music();
                    is_pause = cn_false;
                }
				break;
            }
            return 0;
        }
        else if(event.type == SDL_QUIT)
            return 1;
    }
}
