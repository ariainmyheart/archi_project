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
#include "error.h"

struct cpu_struct* init_cpu()
{
	struct cpu_struct* cpu = (struct cpu_struct*)malloc(sizeof(struct cpu_struct));
	memset(cpu, 0, sizeof(struct cpu_struct));
	cpu->pc = init_byte_array(cpu->ins, 'i');
	cpu->reg[29] = init_byte_array(cpu->mem, 'd');

	int i;
	for (i = 0; i < 5; i++) {
		cpu->pipeline[i].is_nop = 1;
		strcpy(cpu->pipeline[i].ins.name, "NOP");
	}
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

void make_nop(struct pipe_struct* pipe)
{
	memset(pipe, 0, sizeof(struct pipe_struct));
	pipe->is_nop = 1;
	strcpy(pipe->ins.name, "NOP");
}

int cpu_cycle(struct cpu_struct* cpu)
{
	int status = 0;

	write_back(cpu, &status);
	data_mem(cpu, &status);
	execute(cpu, &status);
	ins_decode(cpu);
	ins_fetch(cpu);

	return status;

	// DEBUG("----------------\n\n");
}

void cpu_next_cycle(struct cpu_struct* cpu)
{
	int i;
	for (i = 4; i > 0; i--) {
		if (cpu->pipeline[i-1].stall) {
			make_nop(&cpu->pipeline[i]);
			cpu->pipeline[i-1].stall--;
			cpu->pc = cpu->last_pc;
			break;
		}
		cpu->pipeline[i] = cpu->pipeline[i-1];
	}
	if (cpu->pipeline[IF].flush) {
		make_nop(&cpu->pipeline[ID]);
		cpu->pipeline[IF].flush--;
	}
}

void ins_fetch(struct cpu_struct* cpu)
{
	int flush = cpu->pipeline[IF].flush;
	memset(&cpu->pipeline[IF], 0, sizeof(struct pipe_struct));
	cpu->pipeline[IF].flush = flush;

	word_t ins = 0;
	int i;
	for (i = 0; i < 4; i++) {
		ins <<= 8;
		ins |= cpu->ins[i+cpu->pc];
	}
	cpu->pipeline[IF].ins.hex = ins;
	cpu->pipeline[IF].is_nop = 0;
	cpu->last_pc = cpu->pc;
	cpu->pc += 4;

	cpu->pipeline[IF].ins.pc = cpu->pc;

	if (cpu->pipeline[IF].flush) {
		if (is_branch(cpu->pipeline[ID].ins))
			cpu->pc +=  cpu->pipeline[ID].ins.imm * 4 - 4;
		else if (is_jump(cpu->pipeline[ID].ins)) {
			if (cpu->pipeline[ID].ins.op == 0 && cpu->pipeline[ID].ins.funct == JR)
				cpu->pc = cpu->pipeline[ID].data1.value;
			else
				cpu->pc = ((cpu->pc-4)&0xf0000000) | (cpu->pipeline[ID].ins.immu*4);
		}
	}

	// DEBUG("IF\n");
	// DEBUG("\t0x%08x\n", ins);
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

	get_ins_name(&cpu->pipeline[ID].ins, cpu->pipeline[ID].is_nop);

	if (is_sll_0_0_0(cpu->pipeline[ID].ins)) {
		make_nop(&cpu->pipeline[ID]);
		return;
	}

	get_data_1(cpu);
	get_data_2(cpu);
	get_write_data(cpu);
	cpu->pipeline[ID].write_reg = get_write_reg(cpu);

	check_stall(cpu);
	if (cpu->pipeline[ID].stall)
		return;

	check_EX_DM_to_ID_fwd(cpu);

	if (is_branch(cpu->pipeline[ID].ins)) {
		struct data_info* data1 = &cpu->pipeline[ID].data1;
		struct data_info* data2 = &cpu->pipeline[ID].data2;
		if (cpu->pipeline[ID].ins.op == BEQ && data1->value == data2->value)
			cpu->pipeline[IF].flush = 1;
		if (cpu->pipeline[ID].ins.op == BNE && data1->value != data2->value)
			cpu->pipeline[IF].flush = 1;
	}
	if (is_jump(cpu->pipeline[ID].ins)) {
		cpu->pipeline[IF].flush = 1;
	}

	// DEBUG("ID\n");
	// DEBUG("\t0x%08x\n", cpu->pipeline[ID].ins.hex);
	// DEBUG("\tins: %s\n", cpu->pipeline[ID].ins.name);
	// DEBUG("\tdata1: 0x%08x\n", cpu->pipeline[ID].data1.value);
	// DEBUG("\tdata2: 0x%08x\n", cpu->pipeline[ID].data2.value);
	// DEBUG("\twrite_reg: %d\n", cpu->pipeline[ID].write_reg);
}

void execute(struct cpu_struct* cpu, int* status)
{
	if (cpu->pipeline[EX].is_nop)
		return;

	check_DM_WB_to_EX_fwd(cpu);
	check_EX_DM_to_EX_fwd(cpu);
	*status |= alu_calculate(cpu);

	/* DEBUG("EX\n"); */
	/* DEBUG("\tins: %s\n", cpu->pipeline[EX].ins.name); */
	/* DEBUG("\tdata1: 0x%08x\n", cpu->pipeline[EX].data1.value); */
	/* DEBUG("\tdata1_fwd: %d\n", cpu->pipeline[EX].data1.fwd.has_fwd); */
	/* DEBUG("\tdata2: 0x%08x\n", cpu->pipeline[EX].data2.value); */
	/* DEBUG("\tdata2_fwd: %d\n", cpu->pipeline[EX].data2.fwd.has_fwd); */
}

void data_mem(struct cpu_struct* cpu, int* status)
{
	if (cpu->pipeline[DM].is_nop)
		return;

	word_t* read_data = &cpu->pipeline[DM].read_data;
	word_t write_data = cpu->pipeline[DM].write_data.value;
	word_t addr = cpu->pipeline[DM].alu_result;

	switch (cpu->pipeline[DM].ins.op) {
		case LW:
			*read_data = load_memory(cpu, addr, 4, status);
			break;
		case LH:
			*read_data = load_memory(cpu, addr, 2, status);
			*read_data = sign_extend(*read_data, 16);
			break;
		case LHU:
			*read_data = load_memory(cpu, addr, 2, status);
			break;
		case LB:
			*read_data = load_memory(cpu, addr, 1, status);
			*read_data = sign_extend(*read_data, 8);
			break;
		case LBU:
			*read_data = load_memory(cpu, addr, 1, status);
			break;
		case SW:
			 save_memory(cpu, write_data, addr, 4, status);
			 break;
		case SH:
			 save_memory(cpu, write_data, addr, 2, status);
			 break;
		case SB:
			 save_memory(cpu, write_data, addr, 1, status);
			 break;
	}

	// DEBUG("DM\n");
	// DEBUG("\t0x%08x\n", cpu->pipeline[DM].ins.hex);
	// DEBUG("\tins: %s\n", cpu->pipeline[DM].ins.name);
	// DEBUG("\taddr: 0x%08x\n", addr);
	// DEBUG("\tread_data: 0x%08x\n", *read_data);
	// DEBUG("\twrite_data: 0x%08x\n", write_data);

}

void write_back(struct cpu_struct* cpu, int* status)
{
	if (cpu->pipeline[WB].is_nop)
		return;

	word_t write_data = which_write_data(cpu);
	int write_reg = cpu->pipeline[WB].write_reg;

	if (has_write_reg(cpu->pipeline[WB].ins)) {
		if (write_reg == 0) {
			*status |= WRITE_REG_ZERO;
		} else {
			cpu->reg[write_reg] = write_data;
		}
	}

	// DEBUG("WB\n");
	// DEBUG("\t0x%08x\n", cpu->pipeline[WB].ins.hex);
	// DEBUG("\tins: %s\n", cpu->pipeline[WB].ins.name);
	// DEBUG("\thas_write_reg: %d\n", has_write_reg(cpu->pipeline[WB].ins));
	// DEBUG("\twrite_data: %d\n", write_data);
	// DEBUG("\twrite_reg: %d\n", write_reg);
}

