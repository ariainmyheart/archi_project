#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "readfile.h"
#include "instruction.h"
#include "alu.h"
#include "memory.h"
#include "control.h"
#include "debug.h"

struct cpu_struct* init_cpu()
{
	struct cpu_struct* cpu = (struct cpu_struct*)malloc(sizeof(struct cpu_struct));
	memset(cpu, 0, sizeof(struct cpu_struct));
	cpu->pc = init_byte_array(cpu->ins, 'i');
	cpu->reg[29] = init_byte_array(cpu->mem, 'd');
	return cpu;
}

void free_cpu(struct cpu_struct* cpu)
{
	free(cpu);
}

int is_halt(struct cpu_struct* cpu)
{
	if (cpu->pipeline[WB].ins.op == HALT)
		return 1;
	return 0;
}

void cpu_cycle(struct cpu_struct* cpu)
{
	int i;
	write_back(cpu);
	data_mem(cpu);
	execute(cpu);
	ins_decode(cpu);
	ins_fetch(cpu);
	for (i = 4; i > 0; i--) {
		cpu->pipeline[i] = cpu->pipeline[i-1];
	}

	DEBUG("----------------\n\n");
}

void ins_fetch(struct cpu_struct* cpu)
{
	word_t ins = 0;
	int i;
	for (i = 0; i < 4; i++) {
		ins <<= 8;
		ins |= cpu->ins[i+cpu->pc];
	}
	cpu->pipeline[IF].ins.hex = ins;
	cpu->pc += 4;

	DEBUG("IF\n");
	DEBUG("\t0x%08x\n", ins);
}

int sign_extend(int num, int len)
{
	return (num << (32-len)) >> (32-len);
}

int extract(word_t ins, int left, int right)
{
	unsigned int mask = -1;
	return (ins >> right) & (mask >> (31-left+right));
}

void ins_decode(struct cpu_struct* cpu)
{
	word_t ins = cpu->pipeline[ID].ins.hex;
	cpu->pipeline[ID].ins.op = extract(ins, 31, 26);
	cpu->pipeline[ID].ins.rs = extract(ins, 25, 21);
	cpu->pipeline[ID].ins.rt = extract(ins, 20, 16);
	cpu->pipeline[ID].ins.rd = extract(ins, 15, 11);
	cpu->pipeline[ID].ins.shamt = extract(ins, 10, 6);
	cpu->pipeline[ID].ins.funct = extract(ins, 5, 0);
	cpu->pipeline[ID].ins.imm = sign_extend(extract(ins, 15, 0), 16);
	cpu->pipeline[ID].ins.immu = extract(ins, 15, 0);
	cpu->pipeline[ID].ins.addr = extract(ins, 25, 0);

	get_ins_name(&cpu->pipeline[ID].ins);

	cpu->pipeline[ID].data1 = get_data_1(cpu);
	cpu->pipeline[ID].data2 = get_data_2(cpu);
	cpu->pipeline[ID].write_reg = get_write_reg(cpu);

	DEBUG("ID\n");
	DEBUG("\t0x%08x\n", cpu->pipeline[ID].ins.hex);
	DEBUG("\tins: %s\n", cpu->pipeline[ID].ins.name);
	DEBUG("\tdata1: 0x%08x\n", cpu->pipeline[ID].data1);
	DEBUG("\tdata2: 0x%08x\n", cpu->pipeline[ID].data2);
	DEBUG("\twrite_reg: %d\n", cpu->pipeline[ID].write_reg);
}

void execute(struct cpu_struct* cpu)
{
	alu_calculate(cpu);

	DEBUG("EX\n");
	DEBUG("\t0x%08x\n", cpu->pipeline[EX].ins.hex);
	DEBUG("\tins: %s\n", cpu->pipeline[EX].ins.name);
	DEBUG("\tdata1: 0x%08x\n", cpu->pipeline[EX].data1);
	DEBUG("\tdata2: 0x%08x\n", cpu->pipeline[EX].data2);
	DEBUG("\talu_result: 0x%08x\n", cpu->pipeline[EX].alu_result);
}

void data_mem(struct cpu_struct* cpu)
{
	word_t* read_data = &cpu->pipeline[DM].read_data;
	word_t write_data = cpu->pipeline[DM].write_data;
	word_t addr = cpu->pipeline[DM].alu_result;

	switch (cpu->pipeline[DM].ins.op) {
		case LW:
			*read_data = load_memory(cpu, addr, 4);
			break;
		case LH:
			*read_data = load_memory(cpu, addr, 2);
			*read_data = sign_extend(*read_data, 16);
			break;
		case LHU:
			*read_data = load_memory(cpu, addr, 2);
			break;
		case LB:
			*read_data = load_memory(cpu, addr, 1);
			*read_data = sign_extend(*read_data, 8);
			break;
		case LBU:
			*read_data = load_memory(cpu, addr, 1);
			break;
		case SW:
			 save_memory(cpu, write_data, addr, 4);
			 break;
		case SH:
			 save_memory(cpu, write_data, addr, 2);
			 break;
		case SB:
			 save_memory(cpu, write_data, addr, 1);
			 break;
	}

	DEBUG("DM\n");
	DEBUG("\t0x%08x\n", cpu->pipeline[DM].ins.hex);
	DEBUG("\tins: %s\n", cpu->pipeline[DM].ins.name);
	DEBUG("\taddr: 0x%08x\n", addr);
	DEBUG("\tread_data: 0x%08x\n", *read_data);
	DEBUG("\twrite_data: 0x%08x\n", write_data);

}

void write_back(struct cpu_struct* cpu)
{

	word_t write_data = which_write_data(cpu);
	int write_reg = cpu->pipeline[WB].write_reg;

	if (has_write_reg(cpu->pipeline[WB].ins))
		cpu->reg[write_reg] = write_data;

	DEBUG("WB\n");
	DEBUG("\t0x%08x\n", cpu->pipeline[WB].ins.hex);
	DEBUG("\tins: %s\n", cpu->pipeline[WB].ins.name);
	DEBUG("\thas_write_reg: %d\n", has_write_reg(cpu->pipeline[WB].ins));
	DEBUG("\twrite_data: %d\n", write_data);
	DEBUG("\twrite_reg: %d\n", write_reg);
}

