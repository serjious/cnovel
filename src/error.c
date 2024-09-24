#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "def.h"
#include "error.h"
#include "strutils.h"

static cn_bool is_fatal_error = cn_false;

void fatal_error()
{
	is_fatal_error = cn_true;	
}

char* send_message_error(const char* message)
{
	static char error_message[error_str_size];
	if(message)
		cn_strncpy(error_message, message, error_str_size - 1);
	return error_message;
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
        fprintf(stderr, "[FATAL ERROR]: %s\n", send_message_error(NULL));
		exit(1);
    } else {
        fprintf(stderr, "[ERROR]: %s\n", send_message_error(NULL));
	}
}
/*if input value is cn_invariable, we don't change static int variable*/

