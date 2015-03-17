#include <stdlib.h>
#include "include/cpu.h"
#include "include/instruction.h"

int extract(word_t ins, int left, int right)
{
	unsigned int mask = -1;
	return (ins >> right) & (mask >> (31-left+right));
}

struct ins_struct* decode(word_t ins)
{
	struct ins_struct* dec = (struct ins_struct*)malloc(sizeof(struct ins_struct));
	dec->ins = ins;
	dec->op = extract(ins, 31, 26);
	dec->rs = extract(ins, 25, 21);
	dec->rt = extract(ins, 20, 16);
	dec->rd = extract(ins, 15, 11);
	dec->shamt = extract(ins, 10, 6);
	dec->funct = extract(ins, 5, 0);
	dec->imm = extract(ins, 15, 0);
	return dec;
}
