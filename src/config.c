#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "strutils.h"
#include "error.h"
#include "utils.h"
#include "def.h"
/*in future we will set setting's in menu, and we need function for fine tuning. HERNYA*/

enum {
	cfg_str_size = 32
};

static const char* cfg_arr[] = {
    "width",
    "height",
    "fullscreen",
    "music",
    "effect",
    "voice",
    "textSpeed",
    "autoTransition",
    "speedAutoTransition",
	"textLanguage",
	"voiceLanguage"
};

enum cfg_index {
    i_width,
    i_height,
    i_fullscreen,
    i_music,
    i_effect,
    i_voice,
    i_textSpeed,
    i_autoTransition,
    i_speedAutoTransition,
	i_textLanguage,
	i_voiceLanguage
};

static void set_as_default(config* cfg)
{
	cfg->width = 192;
	cfg->height = 108;
	cfg->fullscreen = 0;
	cfg->music = 50;
	cfg->effect = 50;
	cfg->voice = 50;
	cfg->textSpeed = 50;
	cfg->autoTransition = 1;
	cfg->speedAutoTransition = 50;
	cn_strncpy(cfg->textLanguage, "en", lang_str_size);
	cn_strncpy(cfg->voiceLanguage, "en", lang_str_size);
}

static int check_valid_values(config* cfg)
{
	int i;
	for(i = 0; i<ARRAY_SIZE(cfg_arr); i++)
	{
		if(i == i_width || i == i_height) {
			if(!in_range(window_min_side, window_max_side, *((int*)cfg+i)))
				return -1;
		} else if (i == i_fullscreen || i == i_autoTransition) {
			if(!in_range(bool_false, bool_true, *((int*)cfg+i)))
				return -1;
		} else if(i == i_textLanguage || i == i_voiceLanguage) {
			if(cn_strncmp("en", (char*)cfg+i*sizeof(int), lang_str_size))
				return -1;
		} else {
			if(!in_range(percent_min, percent_max, *((int*)cfg+i)))
				return -1;
		}
	}
	return 0;
}
static int init_video(const char* key, const char* value, config* cfg)
{
	if(0 == cn_strcmp(key, cfg_arr[i_width])) {
		cfg->width = strtol(value, NULL, 10);
		return 0;
	}
	if(0 == cn_strcmp(key, cfg_arr[i_height])) {
		cfg->height = strtol(value, NULL, 10);
		return 0;
	}
	if(0 == cn_strcmp(key, cfg_arr[i_fullscreen])) {
		cfg->fullscreen = strtol(value, NULL, 10);
		return 0;
	}
	return -1;
}

static int init_sound(const char* key, const char* value, config* cfg)
{
	if(0 == cn_strcmp(key, cfg_arr[i_music])) {
		cfg->music = strtol(value, NULL, 10);
		return 0;
	}
	if(0 == cn_strcmp(key, cfg_arr[i_effect])) {
		cfg->effect = strtol(value, NULL, 10);
		return 0;
	}
	if(0 == cn_strcmp(key, cfg_arr[i_voice])) {
		cfg->voice = strtol(value, NULL, 10);
		return 0;
	}
	return -1;
}
static int init_text(const char* key, const char* value, config* cfg)
{
	if(0 == cn_strcmp(key, cfg_arr[i_textSpeed])) {
		cfg->textSpeed = strtol(value, NULL, 10);
		return 0;
	}
	if(0 == cn_strcmp(key, cfg_arr[i_autoTransition])) {
		cfg->autoTransition = strtol(value, NULL, 10);
		return 0;
	}
	if(0 == cn_strcmp(key, cfg_arr[i_speedAutoTransition])) {
		cfg->speedAutoTransition = strtol(value, NULL, 10);
		return 0;
	}
	return -1;
}
static int init_language(const char* key, const char* value, config* cfg)
{
	if(0 == cn_strcmp(key, cfg_arr[i_textLanguage])) {
		cn_strncpy(cfg->textLanguage, value, lang_str_size);
		return 0;
	}
	if(0 == cn_strcmp(key, cfg_arr[i_voiceLanguage])) {
		cn_strncpy(cfg->voiceLanguage, value, lang_str_size);
		return 0;
	}
	return -1;
}

static void init_config(const char* key, const char* value, config* cfg)
{
	if(0 == init_video(key, value, cfg))
		return;
	if(0 == init_sound(key, value, cfg))
		return;
	if(0 == init_text(key, value, cfg))
		return;
	init_language(key, value, cfg);
}

int read_from_config(const char* path, config* cfg)
{
	char buf[cfg_str_size];
	char error_message[error_str_size];
	FILE* fd;

	fd = fopen(path, "r");
	/*print strerror(error)*/
	if(!fd) {
		set_as_default(cfg);
		sprintf(error_message, "%s: %s", path,
		"Can't open file for read, will be used standard settigs");
	
		send_message_error(error_message);
		return -1;
	}

    while(get_string(buf, cfg_str_size, fd))
	{
		char* key;
		char* value;
		key = strtok(buf, " =");
		value = strtok(NULL, " =");
		if(key && value)
			init_config(key, value, cfg);
    }
	fclose(fd);
	if(0 != check_valid_values(cfg)) {
		set_as_default(cfg);
		sprintf(error_message, "%s: %s", path, 
		"Incorrect settings, will be used standard settings");
		send_message_error(error_message);
		return -1;
	}
	return 0;
}

int write_to_config(const char* path, config* cfg)
{
	char error_message[error_str_size];
	FILE* fd;
	int i;
	/*
	if(-1 == check_valid_values(cfg))
		set_as_default(cfg);
	*/
	fd = fopen(path, "w");
	if(!fd) {
		sprintf(error_message, "%s: %s", path, "Can't open file for write");
		fatal_error();
		send_message_error(error_message);
		return -1;
	}

	for(i = 0; i<ARRAY_SIZE(cfg_arr)-2; i++)
		fprintf(fd, "%s = %d\n", cfg_arr[i], *((int*)cfg+i));

	for( ; i<ARRAY_SIZE(cfg_arr); i++)
		fprintf(fd, "%s = %s\n", cfg_arr[i], (char*)cfg+i*sizeof(i));

	fclose(fd);
	return 0;

}
