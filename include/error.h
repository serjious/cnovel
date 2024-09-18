#ifndef CNOVEL_ERROR_H_SENTRY
#define CNOVEL_ERROR_H_SENTRY

#define ERROR_HANDLING(err)	do	\
	if(-1 == err) {				\
		print_error();			\
		if(is_fatal_error())	\
			return 1;			\
	}							\
	while(0)

enum {
	error_str_size = 128
	/*change*/
};
typedef enum m_fatal_arguments {
	cn_fatal = 1,
	cn_ok = 0,
	cn_invariable = -1
} fatal_arguments;

int fatal_error(fatal_arguments boolean);
int is_fatal_error();
char* send_message_error(const char* message);
void print_error();

#endif
