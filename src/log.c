#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "log.h"
#include "def.h"

static cn_bool cn_auto_output = cn_false;

void enable_log()
{
	cn_auto_output = cn_true;
}

void disable_log()
{
	cn_auto_output = cn_false;
}
/*print time*/
int printf_log(const char* format, ...)
{
    int result;
	va_list vl;
	time_t m_time;
	struct tm* cur_t; 

	result = -1;

	if(cn_auto_output) {
		time(&m_time);
		cur_t = localtime(&m_time);
		printf("[%02d:%02d:%02d][LOG]: ",	cur_t->tm_hour,
											cur_t->tm_min,
											cur_t->tm_sec);
        va_start(vl, format);
		result = vprintf(format, vl);
		printf("\n");
        va_end(vl);
	}

	return result;
}
