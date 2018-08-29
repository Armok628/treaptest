#include "timer.h"
static struct timespec set;
void start_timer()
{
	clock_gettime(CLOCK_MONOTONIC,&set);
}
double read_timer()
{
	struct timespec end;
	clock_gettime(CLOCK_MONOTONIC,&end);
	return (end.tv_sec-set.tv_sec)+(end.tv_nsec-set.tv_nsec)/1e9;
}
