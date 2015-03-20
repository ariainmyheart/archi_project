#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/readfile.h"
#include "include/cpu.h"
#include "include/error.h"

int sign_extend(int num, int len)
{
	return (num << (32-len)) >> (32-len);
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
	cpu->current_ins.imm = sign_extend(extract(ins, 15, 0), 16);
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

word_t load_memory(struct cpu_struct* cpu, word_t addr, int byte, int* status)
{
	word_t value = 0;
	int i;
	//if (check_addr_overflow(addr, status)) return 0;
	//if (check_data_align(addr, byte, status)) return 0;
	for (i = 0; i < byte; i++) {
		value <<= 8;
		value |= cpu->mem[addr+i];
	}
	return value;
}

void write_register(struct cpu_struct* cpu, int num, word_t value, int* status)
{
	if (num == 0) {
		*status |= WRITE_REG_ZERO;
	} else {
		cpu->reg[num] = value;
	}
}

