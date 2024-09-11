#include "utils.h"
#include <stdio.h>

void range(int min, int max, int* value)
{
	if(min > max) {
		int tmp;
		tmp = max;
		max = min;
		min = tmp;
	}
	if(*value > max)
		*value = max;
	else if (*value < min)
		*value = min;
}
int in_range(int min, int max, int value)
{
	if(min > max) {
		int tmp;
		tmp = max;
		max = min;
		min = tmp;
	}
	if(value > max || value < min)
		return 0;	
	return 1;
}
void place_in_range(int* value, int min, int max, int req_min, int req_max)
{
	/*Very bad and unsafe function*/
	double ratio = req_max / (double)max;
	*value = (int)*value * ratio;
}
