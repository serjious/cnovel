#ifndef SOUND_H_SENTRY
#define SOUND_H_SENTRY

#include <stdio.h>
#include "config.h"
#include "def.h"

int init_audio(config* cfg);
void set_volume(config* cfg);

int load_music(const char* name, const char* path);
int load_voice(const char* name, const char* path);
int load_effect(const char* name, const char* path);

int halt_music();
int halt_voice();
int halt_effect();

int play_music(const char* name);
int play_voice(const char* name);
int play_effect(const char* name);

void pause_music();
void pause_voice();
void pause_effect();

void resume_music();
void resume_voice();
void resume_effect();

void save_audio_to_file(FILE* fd);
int load_audio_from_file(FILE* fd);

void close_audio();

#endif
