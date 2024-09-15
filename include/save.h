#ifndef SAVE_H_SENTRY
#define SAVE_H_SENTRY

typedef enum m_type_data_save {
	cn_type_data_audio
} cn_type_data_save;

int save(const char* path);
int load(const char* path);

#endif
