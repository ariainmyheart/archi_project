#include <stdio.h>
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

void print_ins(struct ins_struct* ins)
{
	printf("op: %x\n", ins->op);
	printf("rs: %x\n", ins->rs);
	printf("rt: %x\n", ins->rt);
	printf("rd: %x\n", ins->rd);
	printf("shamt: %x\n", ins->shamt);
	printf("funct: %x\n", ins->funct);
	printf("imm: %x\n", ins->imm);
	printf("immu: %x\n", ins->immu);
	printf("addr: %x\n", ins->addr);
}

