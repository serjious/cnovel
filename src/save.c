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
		printf_error("%s: %s", path, strerror(errno));
		return -1;
	}
	save_audio_to_file(fd);
	printf_log("Saved %s", path);
	fclose(fd);
	return 0;
}

int load(const char* path)
{
	int st;
	FILE* fd;

	fd = fopen(path, "rb");
	if(!fd) {
		printf_error("%s: %s", path, strerror(errno));
		return -1;
	}
	printf_log("Load %s", path);
	st = load_audio_from_file(fd);
	if(st == -1) {
		printf_error("Failed to read save %s\n", path);
		return -1;
	}
	fclose(fd);
	return 0;
}
