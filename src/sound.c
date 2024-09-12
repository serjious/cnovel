#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_error.h>
#include "sound.h"
#include "error.h"
#include "utils.h"
#include "strutils.h"
#include "def.h"


enum {
	cn_channel_effect,
	cn_channel_voice
};

enum {
    /*add chunk_size*/
    audio_name_str_size = 128
    /*variable size is not may be different*/
};

typedef enum m_load_type {
    cn_music,
    cn_chunk
} load_type;


typedef struct m_audio {
    char name[audio_name_str_size];
    Mix_Music *music;
    Mix_Chunk *chunk;
    cn_bool is_playing;
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
    return first_elem(NULL, cn_set);
}

static void push_audio(node **first, const char* name,
                       const char* path, load_type type)
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
		tmp->audio->is_playing = cn_false;
		*first = tmp;
    } else {
        push_audio(&(*first)->next, name, path, type);
    }
    /*strncpy FIX!!*/
}

static void delete_audio(node** first)
{
    if(!*first) {
        return;
    } else {
        delete_audio(&(*first)->next);
    }
    Mix_FreeChunk((*first)->audio->chunk);
    Mix_FreeMusic((*first)->audio->music);
    free((*first)->audio);
    free(*first);
    *first = NULL;
}
static cn_audio* search_audio(node* first, const char* name)
{
    node* tmp = first;

    while(tmp)
    {
        if(0 == cn_strcmp(tmp->audio->name, name)) 
           return tmp->audio;
        tmp = tmp->next;
    }
    return NULL;
}



int init_audio(config* cfg)
{
	int init;
	/*enum*/
	int chunk_size = 2048;


	init = SDL_Init(SDL_INIT_AUDIO);

	if(-1 == init) {
        fatal_error(cn_ok);
        send_message_error(Mix_GetError());
        return -1;
	}

	init = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                         MIX_DEFAULT_FORMAT,
                         MIX_DEFAULT_CHANNELS, chunk_size);
    if(-1 == init) {
        fatal_error(cn_ok);
        send_message_error(Mix_GetError());
        return -1;
	}

	set_volume(cfg);
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
static int load_audio(const char* name, const char* path, load_type type)
{
    node* first;
    first = get_first();
    push_audio(&first, name, path, type);
    set_first(first);
    return 0;
}
int load_music(const char* name, const char* path)
{
    return load_audio(name, path, cn_music);
}
int load_voice(const char* name, const char* path)
{
    return load_audio(name, path, cn_chunk);
}
int load_effect(const char* name, const char* path)
{
    return load_audio(name, path, cn_chunk);
}

int play_music(const char* name)
{
    int st;
    node* first = get_first();
    cn_audio* audio = search_audio(first, name);
	if(!audio || !audio->music) {
        fatal_error(cn_ok);
        send_message_error("No music whis this name found");
        return -1;
    }
    st = Mix_PlayMusic(audio->music, cn_loop);
    if(-1 == st) {
        fatal_error(cn_ok);
        send_message_error(Mix_GetError());
        return -1;
    }
    audio->is_playing = cn_true;
    return 0;
	/*error handling*/
}

int play_voice(const char* name)
{
    int st;
    node* first = get_first();
    cn_audio* audio = search_audio(first, name);
	if(!audio || !audio->chunk) {
        fatal_error(cn_ok);
        send_message_error("No voice whis this name found");
        return -1;
    }
    st = Mix_PlayChannel(cn_channel_voice, audio->chunk, cn_unloop);
    if(-1 == st) {
        fatal_error(cn_ok);
        send_message_error(Mix_GetError());
        return -1;
    }
    audio->is_playing = cn_true;
    return 0;
}

int play_effect(const char* name, playback_options opt)
{
    int st;
    node* first = get_first();
    cn_audio* audio = search_audio(first, name);
    if(audio || !audio->chunk) {
        fatal_error(cn_ok);
        send_message_error("No effect whis this name found");
        return -1;
    }
    st = Mix_PlayChannel(cn_channel_voice, audio->chunk, opt);
    if(-1 == st) {
        fatal_error(cn_ok);
        send_message_error(Mix_GetError());
        return -1;
    }
    return 0;
}

int halt_music()
{
    int st;
    st = Mix_HaltMusic();
    if(-1 == st) {
        fatal_error(cn_ok);
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
        fatal_error(cn_ok);
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
        fatal_error(cn_ok);
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

void close_audio()
{
    node* first = get_first();

    halt_music();
    halt_voice();
    halt_effect();

    delete_audio(&first);
    set_first(first);

    Mix_CloseAudio();
    Mix_Quit();
}

