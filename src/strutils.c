#include "strutils.h"
#include <stdio.h>

#define ARRAY_SIZE(arr) sizeof(arr)/sizeof(*(arr))

int cn_strlen(const char* s)
{
	int i;
	for(i = 0; s[i]; i++)
		{}
	return i;
}
int cn_strcmp(const char* s1, const char* s2)
{
	for( ; *s1 || *s2; s1++, s2++)
		if(*s1 != *s2)
			return *s1 - *s2;

	return 0;
}
int cn_strncmp(const char* s1, const char* s2, int n)
{
	int i;
	for(i = 0; i < n && (s1[i] || s2[i]); i++)
		if(s1[i] != s2[i])
			return s1[i] - s2[i];

	return 0;
}
void cn_strcpy(char* dest, const char* src)
{
	for( ; *src; src++, dest++)
		*dest = *src;
}
void cn_strncpy(char* dest, const char* src, int n)
{
	/*do not work correctly*/
	int i;
	for(i = 0; i<n && src[i]; i++) {
		dest[i] = src[i];
	}

	for( ; i<n; i++)
		dest[i] = '\0';
}
void cn_strcat(char* dest, const char* src)
{
	for( ; *dest; dest++)
		{}
	for( ; *src; dest++, src++)
		*dest = *src;
	*dest = '\0';
}
char* cn_strtok(char* str, const char* delim)
{
	return NULL;
}


char* get_string(char* buf, int size, FILE* fd)
{
	int i;
	char* tmp;
    tmp = fgets(buf, size, fd);
    for(i = 0; tmp && i<size && buf[i]; i++) {
        if(buf[i] == '\n') {
            buf[i] = '\0';
            break;
        }
    }
    return tmp;
}
