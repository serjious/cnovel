#ifndef CNOVEL_STRING_H_SENTRY
#define CNOVEL_STRING_H_SENTRY
#include <stdio.h>

int cn_strlen(const char* s);
int cn_strcmp(const char* s1, const char* s2);
int cn_strncmp(const char* s1, const char* s2, int n);
void cn_strcpy(char* dest, const char* src);
void cn_strncpy(char* dest, const char* src, int n);
void cn_strcat(char* dest, const char* src);
char* get_string(char* buf, int size, FILE* fd);


#endif
