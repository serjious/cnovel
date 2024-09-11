#include "error.h"
#include "strutils.h"
#include <error.h>
#include <stdio.h>

int fatal_error(fatal_arguments boolean)
{
	static int boolean_fatal_error = cn_ok;
	if(boolean != cn_invariable)
		boolean_fatal_error = boolean;
	return boolean_fatal_error;
}

int is_fatal_error()
{
	return fatal_error(cn_invariable);
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
    if(is_fatal_error())
        fprintf(stderr, "[FATAL ERROR]: %s\n", send_message_error(NULL));
    else
        fprintf(stderr, "[ERROR]: %s\n", send_message_error(NULL));
}
/*if input value is cn_invariable, we don't change static int variable*/

