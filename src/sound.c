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

typedef enum m_load_type {
    cn_music,
    cn_chunk
} load_type;

enum m_playback_opt {
	cn_loop = -1,
	cn_unloop = 1 
};

enum {
	chunk_size = 2048,
	audio_name_str_size = 128,
	path_str_size = 128 /*change*/
    /*variable size is not may be different*/
};

typedef struct m_audio_title {
	cn_type_data_save type;
	int len;
} cn_audio_title;

typedef struct m_audio {
	char name[audio_name_str_size];
	char path[path_str_size];

	load_type type;
	cn_audio_channel channel;

	cn_bool is_playing;
	
	void* ptr_music;
} cn_audio;

typedef struct m_node {
    cn_audio* audio;
    struct m_node* next;
} node;


static node* glb_first_node = NULL;

static void set_first(node* new_first)
{
	glb_first_node = new_first;
}

static node* get_first()
{
    return glb_first_node;
}

static void push_audio(node** first, const char* name,
                       const char* path, load_type type,
					   cn_audio_channel channel)
{

	if(!*first) {
		node* tmp;
		tmp = malloc(sizeof(node));
		tmp->audio = malloc(sizeof(cn_audio));
		tmp->next = NULL;

		if(type == cn_music) {
			tmp->audio->ptr_music = Mix_LoadMUS(path);
		} else {
            tmp->audio->ptr_music = Mix_LoadWAV(path);
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

    delete_audio(&(*first)->next);

	if((*first)->audio->type == cn_music)
		Mix_FreeMusic((*first)->audio->ptr_music);
	else
		Mix_FreeChunk((*first)->audio->ptr_music);
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
		fatal_error();
		printf_error(Mix_GetError());
		return -1;
	}
	init = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                         MIX_DEFAULT_FORMAT,
                         MIX_DEFAULT_CHANNELS,
                         chunk_size);
	if(-1 == init) {
		fatal_error();
		printf_error(Mix_GetError());
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

static int load_audio(const char* name, const char* path,
					  load_type type, cn_audio_channel channel)
{
    node* first;
	cn_audio* tmp;
    first = get_first();
	tmp = search_audio(first, name);
	if(tmp) {
		printf_error("Audio with this name(%s) already exists", name);
		return -1;
	}
    push_audio(&first, name, path, type, channel);
	/*delete*/
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

	if(!audio || !audio->ptr_music || audio->channel != cn_channel_music) {
        printf_error("No music whis this name found");
        return -1;
    }
	audio->channel = cn_channel_music;
    st = Mix_PlayMusic(audio->ptr_music, cn_loop);
    if(-1 == st) {
        printf_error(Mix_GetError());
        return -1;
    }
    audio->is_playing = cn_true;
	printf_log("Play music: %s", name);
    return 0;
}

int play_voice(const char* name)
{
    int st;
    node* first;
    cn_audio* audio;

	first = get_first();
    audio = search_audio(first, name);

	if(!audio || !audio->ptr_music|| audio->channel != cn_channel_voice) {
        printf_error("No voice whis this name found");
        return -1;
    }
	audio->channel = cn_channel_voice;
    st = Mix_PlayChannel(audio->channel, audio->ptr_music, cn_unloop);
    if(-1 == st) {
        printf_error(Mix_GetError());
        return -1;
    }
    audio->is_playing = cn_true;
	printf_log("Play voice: %s", name);
    return 0;
}

int play_effect(const char* name)
{
    int st;
    node* first;
    cn_audio* audio;

	first = get_first();
    audio = search_audio(first, name);

    if(!audio || !audio->ptr_music || audio->channel != cn_channel_effect) {
        printf_error("No effect whis this name found");
        return -1;
    }
	audio->channel = cn_channel_effect;
    st = Mix_PlayChannel(audio->channel, audio->ptr_music, cn_unloop);
    if(-1 == st) {
        printf_error(Mix_GetError());
        return -1;
    }
	audio->is_playing = cn_true;
	printf_log("Play effect: %s", name);
    return 0;
}

int halt_music()
{
    int st;
    st = Mix_HaltMusic();
    if(-1 == st) {
        printf_error(Mix_GetError());
        return -1;
    }
    return 0;
}

int halt_voice()
{
    int st;
    st = Mix_HaltChannel(cn_channel_voice);
    if(-1 == st) {
        printf_error(Mix_GetError());
        return -1;
    }
    return 0;
}

int halt_effect()
{
    int st;
    st = Mix_HaltChannel(cn_channel_effect);
    if(-1 == st) {
        printf_error(Mix_GetError());
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

void save_audio_to_file(FILE* fd)
{
	int len;
	node* first;
	cn_audio_title info;

	first = get_first();
	len = len_audio(first);

	info.type = cn_type_data_audio;
	info.len = len;

	fwrite(&info, 1, sizeof(info), fd);
	while(first) {
		fwrite(first->audio, 1, sizeof(cn_audio), fd);
		first = first->next;
	}	
}
/*make refactoring*/
int load_audio_from_file(FILE* fd)
{
	int i;
	node* first;
	cn_audio_title info;

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
		switch(tmp.channel) {
		case cn_channel_music:
			load_music(tmp.name, tmp.path);
			if(tmp.is_playing)
				play_music(tmp.name);
			break;
		case cn_channel_voice:
			load_voice(tmp.name, tmp.path);
			if(tmp.is_playing)
				play_voice(tmp.name);
			break;
		case cn_channel_effect:
			load_effect(tmp.name, tmp.path);
			if(tmp.is_playing)
				play_effect(tmp.name);
			break;
		}
	}
	return 0;
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

