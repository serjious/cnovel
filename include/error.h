#ifndef CNOVEL_ERROR_H_SENTRY
#define CNOVEL_ERROR_H_SENTRY

#define ERROR_HANDLING(err)	do	\
	if(-1 == err) {				\
		print_error();			\
	}							\
	while(0)

enum {
	error_str_size = 512 
};

void fatal_error();
void printf_error(const char* format, ...);
void print_error();

#endif
