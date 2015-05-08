#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdio.h>

static const int WRITE_REG_ZERO = 1;
static const int NUM_OVERFLOW = 2;
static const int ADDR_OVERFLOW = 4;
static const int DATA_MISALIGNED = 8;

int check_num_overflow(int s, int c, int* status);
int check_addr_overflow(int addr, int byte, int* status);
int check_data_misaligned(int addr, int byte, int* status);
int error_dump(FILE* err, int cycle, int status);

#endif

