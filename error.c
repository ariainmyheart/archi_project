#include "include/cpu.h"
#include "include/error.h"

int check_num_overflow(int s, int c, int* status)
{
	if (s > 0 && c > 0 && s + c < 0) *status |= NUM_OVERFLOW;
	if (s < 0 && c < 0 && s + c > 0) *status |= NUM_OVERFLOW;
	return s + c;
}

int check_addr_overflow(int addr, int* status)
{
	return 0;
}

int check_data_misaligned(int addr, int byte, int* status)
{
	return 0;
}

