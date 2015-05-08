#include "cpu.h"
#include "control.h"
#include "instruction.h"

void check_alu_result_fwd(struct pipe_struct* pipe, struct data_info* data, int from, int to)
{
	if (!has_write_reg(pipe->ins))
		return;
	if (pipe->write_reg == 0)
		return;
	if (pipe->is_nop)
		return;
	if (!data->is_reg)
		return;
	if (data->from_reg == pipe->write_reg) {
		data->fwd.has_fwd = 1;
		data->fwd.from = from;
		data->fwd.to = to;
		data->value = pipe->alu_result;
	}
}

void check_read_data_fwd(struct pipe_struct* pipe, struct data_info* data)
{
	if (pipe->write_reg == 0)
		return;
	if (pipe->is_nop)
		return;
	if (data->from_reg == pipe->write_reg) {
		data->fwd.has_fwd = 1;
		data->fwd.from = WB;
		data->fwd.to = EX;
		data->value = pipe->read_data;
	}
}

void check_EX_DM_to_ID_fwd(struct cpu_struct* cpu)
{
	if (!is_branch(cpu->pipeline[ID].ins) && !is_jump(cpu->pipeline[ID].ins))
		return;
	if (!has_write_reg(cpu->pipeline[EX].ins))
		return;
	if (is_load(cpu->pipeline[EX].ins))
		return;
	check_alu_result_fwd(&cpu->pipeline[DM], &cpu->pipeline[ID].data1, DM, ID);
	check_alu_result_fwd(&cpu->pipeline[DM], &cpu->pipeline[ID].data2, DM, ID);
}

void check_EX_DM_to_EX_fwd(struct cpu_struct* cpu)
{
	if (is_load(cpu->pipeline[EX].ins))
		return;
	if (is_branch(cpu->pipeline[EX].ins))
		return;
	if (is_jump(cpu->pipeline[EX].ins))
		return;
	check_alu_result_fwd(&cpu->pipeline[DM], &cpu->pipeline[EX].data1, DM, EX);
	check_alu_result_fwd(&cpu->pipeline[DM], &cpu->pipeline[EX].data2, DM, EX);
	if (is_save(cpu->pipeline[EX].ins))
		check_alu_result_fwd(&cpu->pipeline[DM], &cpu->pipeline[EX].write_data, DM, EX);
}

void check_DM_WB_to_EX_fwd(struct cpu_struct* cpu)
{
	if (is_branch(cpu->pipeline[EX].ins))
		return;
	if (is_jump(cpu->pipeline[EX].ins))
		return;
	if (is_load(cpu->pipeline[WB].ins)) {
		if (is_save(cpu->pipeline[EX].ins))
			check_read_data_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].write_data);
		check_read_data_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].data1);
		check_read_data_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].data2);
	} else {
		if (is_save(cpu->pipeline[EX].ins))
			check_alu_result_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].write_data, WB, EX);
		check_alu_result_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].data1, WB, EX);
		check_alu_result_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].data2, WB, EX);
	}
}

void check_branch_stall(struct cpu_struct* cpu)
{
	if (!has_write_reg(cpu->pipeline[EX].ins))
		return;
	if (cpu->pipeline[EX].write_reg == 0)
		return;
	if (is_load(cpu->pipeline[EX].ins)) {
		if (cpu->pipeline[EX].write_reg == cpu->pipeline[ID].data1.from_reg)
			cpu->pipeline[ID].stall = 2;
		if (cpu->pipeline[EX].write_reg == cpu->pipeline[ID].data2.from_reg)
			cpu->pipeline[ID].stall = 2;
	} else {
		if (cpu->pipeline[EX].write_reg == cpu->pipeline[ID].data1.from_reg)
			cpu->pipeline[ID].stall = 1;
		if (cpu->pipeline[EX].write_reg == cpu->pipeline[ID].data2.from_reg)
			cpu->pipeline[ID].stall = 1;
	}
}

void check_write_data_stall(struct cpu_struct* cpu)
{
	if (!is_load(cpu->pipeline[EX].ins))
		return;
	if (cpu->pipeline[EX].write_reg == 0)
		return;
	if (cpu->pipeline[EX].write_reg == cpu->pipeline[ID].ins.rt)
		cpu->pipeline[ID].stall = 1;
}

void check_normal_stall(struct cpu_struct* cpu)
{
	if (!is_load(cpu->pipeline[EX].ins))
		return;
	if (cpu->pipeline[EX].write_reg == 0)
		return;
	if (cpu->pipeline[EX].write_reg == cpu->pipeline[ID].data1.from_reg)
		cpu->pipeline[ID].stall = 1;
	if (cpu->pipeline[EX].write_reg == cpu->pipeline[ID].data2.from_reg)
		cpu->pipeline[ID].stall = 1;
}

void check_stall(struct cpu_struct* cpu)
{
	if (cpu->pipeline[ID].is_nop)
		return;

	if (cpu->pipeline[ID].ins.op == 0 && cpu->pipeline[ID].ins.funct == JR) {
		check_branch_stall(cpu);
		return;
	}

	switch (cpu->pipeline[ID].ins.op) {
		case BEQ:
		case BNE:
			check_branch_stall(cpu);
			break;
		case SW:
		case SH:
		case SB:
			check_write_data_stall(cpu);
			break;
		default:
			check_normal_stall(cpu);
			break;
	}
}

