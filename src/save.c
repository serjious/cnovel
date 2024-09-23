#include "save.h"
#include "sound.h"
#include "error.h"
#include "log.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

int save(const char* path)
{
	FILE* fd;
	fd = fopen(path, "wb");
	if(!fd) {
		char error_message[error_str_size];
		sprintf(error_message, "%s: %s", path, strerror(errno));
		fatal_error(cn_ok);
		send_message_error(error_message);
		return -1;
	}

	save_audio_to_file(fd);
	printf_log("Saved %s", path);

	fclose(fd);
	return 0;
}

int load(const char* path)
{
	FILE* fd;
	fd = fopen(path, "rb");
	if(!fd) {
		char error_message[error_str_size];
		sprintf(error_message, "%s: %s", path, strerror(errno));
		fatal_error(cn_ok);
		send_message_error(error_message);
		return -1;
	}

	load_audio_from_file(fd);
	printf_log("Load %s", path);

	fclose(fd);
	return 0;
}
