#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/cpu.h"

struct cpu_struct* alloc_cpu()
{
	struct cpu_struct* cpu = (struct cpu_struct*)malloc(sizeof(struct cpu_struct));
	memset(cpu, 0, sizeof(struct cpu_struct));
	cpu->pc = init_mem(cpu->ins, 'i');
//	init_mem(cpu->mem, 'd');
	return cpu;
}

void free_cpu(struct cpu_struct* cpu)
{
	if (cpu->current_ins)
		free(cpu->current_ins);
	free(cpu);
}

word_t fetch(struct cpu_struct* cpu)
{
	word_t ins = 0;
	int i;
	for (i = 0; i < 4; i++) {
		ins <<= 8;
		ins |= cpu->ins[i+cpu->pc];
	}
	return ins;
}

void decode(struct cpu_struct* cpu, word_t ins)
{
	struct ins_struct* dec = (struct ins_struct*)malloc(sizeof(struct ins_struct));
	dec->ins = ins;
	dec->op = extract(dec->ins, 31, 26);
	dec->rs = extract(dec->ins, 25, 21);
	dec->rt = extract(dec->ins, 20, 16);
	dec->rd = extract(dec->ins, 15, 11);
	dec->shamt = extract(dec->ins, 10, 6);
	dec->funct = extract(dec->ins, 5, 0);
	dec->imm = extract(dec->ins, 15, 0);
	dec->addr = extract(dec->ins, 25, 0);
        cpu->current_ins = dec;
}

void execute(struct cpu_struct* cpu)
{
	cpu->pc += 4;
	printf("ins: %08x\n", cpu->current_ins->ins);
	printf("\top: %x\n", cpu->current_ins->op);
	printf("\trs: %x\n", cpu->current_ins->rs);
	printf("\trt: %x\n", cpu->current_ins->rt);
	printf("\trd: %x\n", cpu->current_ins->rd);
	printf("\tshamt: %x\n", cpu->current_ins->shamt);
	printf("\tfunct: %x\n", cpu->current_ins->funct);
	printf("\timm: %x\n", cpu->current_ins->imm);
	printf("\taddr: %x\n", cpu->current_ins->addr);
}

