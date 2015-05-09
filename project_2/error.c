#include "cpu.h"
#include "error.h"

int check_num_overflow(int s, int c, int* status)
{
	int mask = 0x80000000;
	int a = (s & mask) ? 1 : 0;
	int b = (c & mask) ? 1 : 0;
	int x = ((s+c) & mask) ? 1 : 0;
	if (a == b && a != x) *status |= NUM_OVERFLOW;
	return s + c;
}

int check_addr_overflow(int addr, int byte, int* status)
{
	int i;
	for (i = 0; i < byte; i++)
		if (addr+i >= 1024 || addr+i < 0) {
			*status |= ADDR_OVERFLOW;
			return 1;
		}
	return 0;
}

int check_data_misaligned(int addr, int byte, int* status)
{
	if (addr % byte != 0) {
		*status |= DATA_MISALIGNED ;
		return 1;
	}
	return 0;
}

int error_dump(FILE* err, int cycle, int status)
{
	int flag = 0;
	if (status & WRITE_REG_ZERO) {
		fprintf(err, "In cycle %d: Write $0 Error\n", cycle);
	}
	if (status & ADDR_OVERFLOW) {
		fprintf(err, "In cycle %d: Address Overflow\n", cycle);
		flag = 1;
	}
	if (status & DATA_MISALIGNED) {
		fprintf(err, "In cycle %d: Misalignment Error\n", cycle);
		flag = 1;
	}
	if (status & NUM_OVERFLOW) {
		fprintf(err, "In cycle %d: Number Overflow\n", cycle);
	}
	return flag;
}

