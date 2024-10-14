#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"
#include "def.h"


static cn_bool cn_auto_output = cn_false;


static void print_info()
{
	int i;
	printf("CNOVEL - visual novel engine\n");
#ifdef BUILD_COUNT
	printf("Build: %ld\n", BUILD_COUNT);
#endif
	for(i = 0; i < 80; i++)
		putchar('-');
	putchar('\n');
}

void enable_log()
{
	cn_auto_output = cn_true;
	print_info();	
}

void disable_log()
{
	cn_auto_output = cn_false;
}

void printf_log(const char* format, ...)
{
	va_list vl;
	time_t m_time;
	struct tm* cur_t; 

	if(cn_auto_output) {
		time(&m_time);
		cur_t = localtime(&m_time);
		printf("[%02d:%02d:%02d][LOG]: ",	cur_t->tm_hour,
											cur_t->tm_min,
											cur_t->tm_sec);
        va_start(vl, format);
		vprintf(format, vl);
        va_end(vl);
		putchar('\n');
	}
}
