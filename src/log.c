#include <stdio.h> 
#include "log.h"
#include "def.h"

static cn_bool cn_auto_output = cn_false;

void enable_log()
{
	cn_auto_output = cn_true;	
}

void send_message_log(const char* message)
{
	if(cn_auto_output)
		printf("[LOG]: %s\n", message);	
}

