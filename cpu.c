#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/readfile.h"
#include "include/cpu.h"

int bit_extend(int bit, int offset)
{
	return (bit << (32-offset)) >> offset;
}

struct cpu_struct* alloc_cpu()
{
	struct cpu_struct* cpu = (struct cpu_struct*)malloc(sizeof(struct cpu_struct));
	memset(cpu, 0, sizeof(struct cpu_struct));
	cpu->pc = init_mem(cpu->ins, 'i');
	cpu->reg[29] = init_mem(cpu->mem, 'd');
	return cpu;
}

void free_cpu(struct cpu_struct* cpu)
{
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
	cpu->current_ins.ins = ins;
	cpu->current_ins.op = extract(ins, 31, 26);
	cpu->current_ins.rs = extract(ins, 25, 21);
	cpu->current_ins.rt = extract(ins, 20, 16);
	cpu->current_ins.rd = extract(ins, 15, 11);
	cpu->current_ins.shamt = extract(ins, 10, 6);
	cpu->current_ins.funct = extract(ins, 5, 0);
	cpu->current_ins.imm = bit_extend(extract(ins, 15, 0), 16);
	cpu->current_ins.immu = extract(ins, 15, 0);
	cpu->current_ins.addr = extract(ins, 25, 0);
}

void execute(struct cpu_struct* cpu)
{
	cpu->pc += 4;
	switch (cpu->current_ins.op) {
		case 0:
			r_type(cpu);
			break;
		case 2:
		case 3:
			j_type(cpu);
			break;
		default:
			i_type(cpu);
			break;
	}
}

