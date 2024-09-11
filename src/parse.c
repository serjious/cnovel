#include "parse.h"
#include "error.h"
#include "strutils.h"
#include <stdio.h>
enum {
	scpt_str_size = 512
};

int read_from_script(const char* path)
{
	char buf[scpt_str_size];
	FILE* fd; 
	fd = fopen(path, "r");

	if(!fd)
		return -1;
	
	while(get_string(buf, scpt_str_size, fd))
	{
		if(buf[0] == '#')
			continue;
		puts(buf);
	}
	fclose(fd);
	return 0;
}
