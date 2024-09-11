#ifndef UTILS_H_SENTRY
#define UTILS_H_SENTRY

void range(int min, int max, int* value);
int in_range(int min, int max, int value);
void place_in_range(int* value, int min, int max, int req_min, int req_max);

#endif
