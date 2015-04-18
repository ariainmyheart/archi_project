#include <stdio.h>
#include <assert.h>
#include "../include/readfile.h"
#include "../include/cpu.h"
#include "../include/instruction.h"
#include "../include/error.h"
#include "test.h"

void test_num_overflow()
{
	int a = 0x7fffffff;
	int b = 0x80000000;
	int status = 0;
	int c = check_num_overflow(a, b, &status);
	printf("%d, %d\n", c, status);
}

