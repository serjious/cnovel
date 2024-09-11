#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "config.h"
#include "window.h"
#include "sound.h"
#include "input.h"
#include "menu.h"


int main(int argc, char* argv[])
{
    int st;
    int frames;
    config* cfg;

    if(argc < 2) {
        fprintf(stderr, "argv err\n");
        return 1;
    }

    cfg = calloc(sizeof(config), 1);

    st = read_from_config(argv[1], cfg);
    ERROR_HANDLING(st);

    st = init_window(cfg);
    ERROR_HANDLING(st);

    st = init_audio(cfg);
    ERROR_HANDLING(st);

    load_music("1", argv[2]);
    st = play_music("1");
    ERROR_HANDLING(st);
    while(run_window())
    {
        prepareScene();
        int quit;
        quit = key_handler();
        if(quit)
            break;
        blit();
        presentScene();
    }
    close_audio();
    close_window();
    free(cfg);
	return 0;
}
/*
int main(int argc, char* argv[])
{
    int c;
    int st;
    int quit = 0;

    config* cfg = calloc(sizeof(config), 1);
    config* cfg = calloc(sizeof(config), 1);

    init_audio();

    st = read_from_config(argv[1], cfg);
    ERROR_HANDLING(st);

    set_volume(cfg);

    st = load_music("1", argv[2]);
    ERROR_HANDLING(st);

    st = load_effect("2", argv[3]);
    ERROR_HANDLING(st);

    st = load_voice("3", argv[4]);
    ERROR_HANDLING(st);


    while((c = getchar()) != EOF && quit == 0)
    {
        switch(c) {
        case '1':
            st = play_music("1");
            ERROR_HANDLING(st);
            break;
        case '2':
            pause_music();
            break;
        case '3':
            resume_music();
            break;
        case '4':
            close_audio();
            break;
        case 'q':
            quit = 1;
            break;
        }
    }

    free(cfg);
	return 0;
}
*/
