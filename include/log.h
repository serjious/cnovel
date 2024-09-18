#ifndef LOG_H_SENTRY
#define LOG_H_SENTRY

enum {
	log_str_size = 128
};

void enable_log();

void send_message_log(const char* message);

#endif
