#ifndef LOG_H_SENTRY
#define LOG_H_SENTRY

enum {
	log_str_size = 128
};

void enable_log();
void disable_log();

int printf_log(const char* format, ...);

#endif
