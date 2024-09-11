#ifndef CNOVEL_CONFIG_H_SENTRY
#define CNOVEL_CONFIG_H_SENTRY
typedef enum m_acceptable_values {
	window_max_side = 5000,
	window_min_side = 100,
	bool_false =  0,
	bool_true = 1,
	percent_min = 1,
	percent_max = 100
} acceptable_values;

enum {
	lang_str_size = 4
};

typedef struct m_config {
	int width;
	int height;
    int fullscreen;

    int music;
    int effect;
    int voice;

    int textSpeed;
    int autoTransition;
    int speedAutoTransition;

	char textLanguage[lang_str_size];
	char voiceLanguage[lang_str_size];
} config;

int read_from_config(const char* name, config* cfg);

int write_to_config(const char* name, config* cfg);

#endif
