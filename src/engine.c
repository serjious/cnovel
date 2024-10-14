#include <stdio.h>
#include <stdlib.h>
#include "engine.h"
#include "config.h"
#include "window.h"
#include "sound.h"
#include "input.h"
#include "menu.h"
#include "save.h"
#include "log.h"
#include "def.h"

static void can_input()
{
	printf("[DEBUG CONSOLE]& ");
}

void init_cnovel(int argc, char* argv[])
{
	
	config* cfg;
	/*
	if(argc < 4) {
		fprintf(stderr, "argv error\n");
		exit(1);
	}
	*/
	enable_log();
	cfg = calloc(sizeof(config), 1);
	/*
	read_from_config(argv[1], cfg);
	*/
	init_window();	
	/*	
	init_audio(cfg);
	*/
}
void example_window()
{
	while(glb_running) {
		int c;
		can_input();
		c = getchar();
		if(c == EOF || c == 'q')
			break;
	}
	close_window();
}
void example_load(int argc, char* argv[])
{
	while(1) {
		int c;
		can_input();
		c = getchar();
		if(c == EOF || c == 'q')
			break;
		if(c == 'l') {
			load("game/saves/save_cnovel.bin");
		if(c == 'h')
			halt_music();
		}
	}
	close_audio();
}
void example_save(int argc, char* argv[])
{
	
	load_music("1", argv[2]);
	load_music("2", argv[3]);
	load_music("3", argv[4]);
	load_music("4", argv[5]);
	
	play_music("4");
	while(1) {
		int c;
		can_input();
		c = getchar();
		if(c == '\n')
			putchar('\n');
		if(c == EOF || c == 'q')
			break;
		if(c == 's') 
			save("game/saves/save_cnovel.bin");
		if(c == 'n')
			play_music("3");
		
	}
	close_audio();
}


