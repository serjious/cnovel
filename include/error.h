#ifndef CNOVEL_ERROR_H_SENTRY
#define CNOVEL_ERROR_H_SENTRY

#define ERROR_HANDLING(err)	do	\
	if(-1 == err) {				\
		print_error();			\
	}							\
	while(0)

enum {
	error_str_size = 128
	/*change*/
};

void fatal_error();
char* send_message_error(const char* message);
void print_error();

#endif
