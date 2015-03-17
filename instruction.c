#include <stdlib.h>
#include "include/cpu.h"
#include "include/instruction.h"

int extract(word_t ins, int left, int right)
{
	unsigned int mask = -1;
	return (ins >> right) & (mask >> (31-left+right));
}

int is_halt(word_t ins)
{
	return extract(ins, 31, 26) == HALT;
}

