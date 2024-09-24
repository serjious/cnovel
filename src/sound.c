#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_error.h>
#include "sound.h"
#include "error.h"
#include "utils.h"
#include "strutils.h"
#include "def.h"
#include "save.h"
#include "log.h"


typedef enum m_audio_channel{
	cn_channel_music,
	cn_channel_effect,
	cn_channel_voice
} cn_audio_channel;


enum {
	chunk_size = 2048,
	audio_name_str_size = 128,
	path_str_size = 128 /*change*/
    /*variable size is not may be different*/
};


typedef enum m_load_type {
    cn_music,
    cn_chunk
} load_type;
/*maybe mv save.h*/


/*is title, not info*/
typedef struct m_audio_info {
	cn_type_data_save type;
	int len;
} cn_audio_info;


typedef struct m_audio {
	char name[audio_name_str_size];
	char path[path_str_size];

	load_type type;
	cn_audio_channel channel;

	cn_bool is_playing;

    Mix_Music* music;
    Mix_Chunk* chunk;
} cn_audio;


typedef struct m_node {
    cn_audio* audio;
    struct m_node* next;
} node;


static node* first_elem(node* new_first, variable_control boolean)
{
    static node* first = NULL;
    if(boolean == cn_set) {
        node* tmp = first;
        first = new_first;
        return tmp;
    }
    return first;
}


static void set_first(node* new_first)
{
    first_elem(new_first, cn_set);
}

static node* get_first()
{
    return first_elem(NULL, cn_get);
}

static void push_audio(node **first, const char* name,
                       const char* path, load_type type,
					   cn_audio_channel channel)
{

	if(!*first) {
		node* tmp;
		tmp = malloc(sizeof(node));
		tmp->audio = malloc(sizeof(cn_audio));
		tmp->next = NULL;

		if(type == cn_music) {
			tmp->audio->music = Mix_LoadMUS(path);
			tmp->audio->chunk = NULL;
		} else {
            tmp->audio->chunk = Mix_LoadWAV(path);
			tmp->audio->music = NULL;
		}
		cn_strncpy(tmp->audio->name, name, audio_name_str_size);
		cn_strncpy(tmp->audio->path, path, path_str_size);
		tmp->audio->type = type;
		tmp->audio->channel = channel;
		tmp->audio->is_playing = cn_false;
		*first = tmp;
    } else {
        push_audio(&(*first)->next, name, path, type, channel);
    }
    /*strncpy FIX!!*/
}

static void delete_audio(node** first)
{
    if(!*first)
        return;
    else
        delete_audio(&(*first)->next);

    Mix_FreeChunk((*first)->audio->chunk);
    Mix_FreeMusic((*first)->audio->music);
    free((*first)->audio);
    free(*first);
    *first = NULL;
}

static int len_audio(node *first)
{
	node* tmp = first;
	int count = 0;
	while(tmp) {
		count++;
		tmp = tmp->next;
	}
	return count;
}

static cn_audio* search_audio(node* first, const char* name)
{
    node* tmp = first;

    while(tmp) {
        if(0 == cn_strcmp(tmp->audio->name, name))
           return tmp->audio;
        tmp = tmp->next;
    }
    return NULL;
}

int init_audio(config* cfg)
{
	int init;


	init = SDL_Init(SDL_INIT_AUDIO);

	if(-1 == init) {
		send_message_error(Mix_GetError());
		return -1;
	}

	init = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                         MIX_DEFAULT_FORMAT,
                         MIX_DEFAULT_CHANNELS,
                         chunk_size);
	if(-1 == init) {
        send_message_error(Mix_GetError());
		return -1;
	}

	set_volume(cfg);
	printf_log("Initiatization audio subsystem");
	return 0;
}

void set_volume(config* cfg)
{
	int music_volume, voice_volume, effect_volume;

	music_volume = cfg->music;
	voice_volume = cfg->voice;
	effect_volume = cfg->effect;

	place_in_range(&music_volume, percent_min, percent_max, 1, 128);
	Mix_VolumeMusic(music_volume);

	place_in_range(&voice_volume, percent_min, percent_max, 1, 128);
	Mix_Volume(cn_channel_voice, voice_volume);

	place_in_range(&effect_volume, percent_min, percent_max, 1, 128);
	Mix_Volume(cn_channel_effect, effect_volume);
}

/* bug we can load a several audio with the same name */
/*return 0?*/
static int load_audio(const char* name, const char* path,
					  load_type type, cn_audio_channel channel)
{
    node* first;
    first = get_first();
    push_audio(&first, name, path, type, channel);
    set_first(first);
    return 0;
}
int load_music(const char* name, const char* path)
{
    return load_audio(name, path, cn_music, cn_channel_music);
}

int load_voice(const char* name, const char* path)
{
    return load_audio(name, path, cn_chunk, cn_channel_voice);
}

int load_effect(const char* name, const char* path)
{
    return load_audio(name, path, cn_chunk, cn_channel_effect);
}

int play_music(const char* name)
{
    int st;
    node* first;
    cn_audio* audio;

	first = get_first();
    audio = search_audio(first, name);
	audio->channel = cn_channel_music;

	if(!audio || !audio->music) {
        send_message_error("No music whis this name found");
        return -1;
    }
    st = Mix_PlayMusic(audio->music, cn_loop);
    if(-1 == st) {
        send_message_error(Mix_GetError());
        return -1;
    }
    audio->is_playing = cn_true;

	printf_log("Play music %s", name);
    return 0;
}

int play_voice(const char* name)
{
    int st;
    node* first;
    cn_audio* audio;

	first = get_first();
    audio = search_audio(first, name);
	audio->channel = cn_channel_voice;

	if(!audio || !audio->chunk) {
        send_message_error("No voice whis this name found");
        return -1;
    }
    st = Mix_PlayChannel(audio->channel, audio->chunk, cn_unloop);
    if(-1 == st) {
        send_message_error(Mix_GetError());
        return -1;
    }
    audio->is_playing = cn_true;

	printf_log("Play voice %s", name);
    return 0;
}

int play_effect(const char* name, playback_options opt)
{
    int st;
    node* first;
    cn_audio* audio;

	first = get_first();
    audio = search_audio(first, name);
	audio->channel = cn_channel_effect;

    if(!audio || !audio->chunk) {
        send_message_error("No effect whis this name found");
        return -1;
    }
    st = Mix_PlayChannel(audio->channel, audio->chunk, opt);
    if(-1 == st) {
        send_message_error(Mix_GetError());
        return -1;
    }
	audio->is_playing = cn_true;

	printf_log("Play effect %s", name);
    return 0;
}

int halt_music()
{
    int st;
    st = Mix_HaltMusic();
    if(-1 == st) {
        send_message_error(Mix_GetError());
        return -1;
    }
    return 0;
}

int halt_voice()
{
    int st;
    st = Mix_HaltChannel(cn_channel_voice);
    if(-1 == st) {
        send_message_error(Mix_GetError());
        return -1;
    }
    return 0;
}

int halt_effect()
{
    int st;
    st = Mix_HaltChannel(cn_channel_effect);
    if(-1 == st) {
        send_message_error(Mix_GetError());
        return -1;
    }
    return 0;
}

void pause_music()
{
	Mix_PauseMusic();
}

void pause_voice()
{
    Mix_Pause(cn_channel_voice);
}

void pause_effect()
{
    Mix_Pause(cn_channel_effect);
}

void resume_music()
{
	Mix_ResumeMusic();
}

void resume_voice()
{
    Mix_Resume(cn_channel_voice);
}

void resume_effect()
{
    Mix_Resume(cn_channel_effect);
}

int save_audio_to_file(FILE* fd)
{
	cn_audio_info info;
	node* first;
	int len;

	first = get_first();
	len = len_audio(first);

	info.type = cn_type_data_audio;
	info.len = len;

	fwrite(&info, 1, sizeof(info), fd);
	while(first) {
		fwrite(first->audio, 1, sizeof(cn_audio), fd);
		first = first->next;
	}
	return 0;
}
/*make refactoring*/
int load_audio_from_file(FILE* fd)
{
	node* first;
	cn_audio_info info;
	int i;

	first = get_first();
	halt_music();
	halt_voice();
	halt_effect();
	delete_audio(&first);

	fread(&info, 1, sizeof(info), fd);
	if(info.type != cn_type_data_audio)
		return -1;
	for(i = 0; i < info.len; i++)
	{
		cn_audio tmp;
		fread(&tmp, 1, sizeof(cn_audio), fd);
		switch(tmp.type) {
		case cn_music:
			load_music(tmp.name, tmp.path);
			if(tmp.is_playing)
				play_music(tmp.name);
			break;
		case cn_chunk:
			switch (tmp.channel) {
			case cn_channel_voice:
				load_voice(tmp.name, tmp.path);
				if(tmp.is_playing)
					play_voice(tmp.name);
				break;
			case cn_channel_effect:
				load_effect(tmp.name, tmp.path);
				if(tmp.is_playing)
					play_effect(tmp.name, cn_loop);
				break;
			}
			break;
		}
	}
}

void close_audio()
{
	node* first;

	first = get_first();

    halt_music();
    halt_voice();
    halt_effect();

    delete_audio(&first);
    set_first(first);

    Mix_CloseAudio();
    Mix_Quit();
	printf_log("Close audio subsystem");
}

