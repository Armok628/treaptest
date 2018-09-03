#include "timer.h"
static clock_t then;
void start_timer()
{
	then=clock();
}
double read_timer()
{
	return (clock()-then)/(double)CLOCKS_PER_SEC;
}
