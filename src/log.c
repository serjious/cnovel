#include <stdio.h>
#include <stdarg.h>
#include "log.h"
#include "def.h"

static cn_bool cn_auto_output = cn_false;
static char log_message[log_str_size];

void enable_log()
{
	cn_auto_output = cn_true;
}

void disable_log()
{
	cn_auto_output = cn_false;
}

int printf_log(const char* format, ...)
{
    int result;
	va_list vl;

	result = -1;

	if(cn_auto_output) {
        va_start(vl, format);
		printf("[LOG]: ");
		result = vprintf(format, vl);
		printf("\n");
        va_end(vl);
	}

	return result;
}

