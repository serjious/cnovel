#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>
#include "def.h"
#include "error.h"
#include "strutils.h"

static cn_bool is_fatal_error = cn_false;
static char error_message[error_str_size] = "No error :)";

void fatal_error()
{
	is_fatal_error = cn_true;	
}

void printf_error(const char* format, ...)
{
	va_list vl;

	va_start(vl, format);
	vsprintf(error_message, format, vl);
	/*vsnprintf in c99 :((*/
	va_end(vl);
	/*auto print error*/
	print_error();
}

void print_error()
{
	time_t m_time;
	struct tm* cur_t;
	
	time(&m_time);	
	cur_t = localtime(&m_time);
	fprintf(stderr, "[%02d:%02d:%02d]", cur_t->tm_hour,
										cur_t->tm_min,
										cur_t->tm_sec);
    if(is_fatal_error)
	{
        fprintf(stderr, "[FATAL ERROR]: %s\n", error_message);
		exit(1);
    } else {
        fprintf(stderr, "[ERROR]: %s\n", error_message);
	}
}

