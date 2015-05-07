#include "cpu.h"
#include "control.h"
#include "instruction.h"

/* int check_EX_DM_to_ID_fwd(struct cpu_struct* cpu) */
/* {} */

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

void check_EX_DM_to_EX_fwd(struct cpu_struct* cpu)
{
	check_alu_result_fwd(&cpu->pipeline[DM], &cpu->pipeline[EX].data1, DM, EX);
	check_alu_result_fwd(&cpu->pipeline[DM], &cpu->pipeline[EX].data2, DM, EX);
}

void check_DM_WB_to_EX_fwd(struct cpu_struct* cpu)
{
	if (is_load(cpu->pipeline[WB].ins)) {
		check_read_data_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].data1);
		check_read_data_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].data2);
	} else {
		check_alu_result_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].data1, WB, EX);
		check_alu_result_fwd(&cpu->pipeline[WB], &cpu->pipeline[EX].data2, WB, EX);
	}
}

