#include <string.h>
#include "cpu.h"
#include "control.h"
#include "instruction.h"

void get_data_1(struct cpu_struct* cpu)
{
	struct data_info* data = &cpu->pipeline[ID].data1;

	int rs = cpu->pipeline[ID].ins.rs;
	int rt = cpu->pipeline[ID].ins.rt;
	int imm = cpu->pipeline[ID].ins.imm;
	switch (cpu->pipeline[ID].ins.op) {
		case 0: /* r_type */
			switch (cpu->pipeline[ID].ins.funct) {
				case SLL:
				case SRL:
				case SRA:
					data->value = cpu->reg[rt];
					data->is_reg = 1;
					data->from_reg = rt;
					data->oprand = 't';
					break;
				default:
					data->value = cpu->reg[rs];
					data->is_reg = 1;
					data->from_reg = rs;
					data->oprand = 's';
					break;
			}
		case 2:
		case 3: /* j_type */
			break;
		case LUI:
			data->value = imm;
			break;
		default: /* i_type */
			data->value = cpu->reg[rs];
			data->is_reg = 1;
			data->from_reg = rs;
			data->oprand = 's';
			break;

	}
}

void get_data_2(struct cpu_struct* cpu)
{
	struct data_info data;
	memset(&data, 0, sizeof(data));

	int rt = cpu->pipeline[ID].ins.rt;
	int shamt = cpu->pipeline[ID].ins.shamt;
	int immu = cpu->pipeline[ID].ins.immu;
	switch (cpu->pipeline[ID].ins.op) {
		case 0: /* r_type */
			switch (cpu->pipeline[ID].ins.funct) {
				case SLL:
				case SRL:
				case SRA:
					data.value = shamt;
					break;
				case JR:
					data.is_reg = 0;
					break;
				default:
					data.value = cpu->reg[rt];
					data.is_reg = 1;
					data.from_reg = rt;
					data.oprand = 't';
					break;
			}
		case 2:
		case 3: /* j_type */
			break;
		case LUI:
			data.value = 16;
			break;
		case BEQ:
		case BNE:
			data.value = cpu->reg[rt];
			data.is_reg = 1;
			data.from_reg = rt;
			data.oprand = 't';
			break;
		default: /* i_type */
			data.value = immu;

	}
	cpu->pipeline[ID].data2 = data;
}

int get_write_reg(struct cpu_struct* cpu)
{
	int rt = cpu->pipeline[ID].ins.rt;
	int rd = cpu->pipeline[ID].ins.rd;
	switch (cpu->pipeline[ID].ins.op) {
		case 0: /* r_type */
			return rd;
		case J:
			return 0;
		case JAL:
			return 31;
		default: /* i_type */
			return rt;

	}
}

int has_write_reg(struct ins_struct ins)
{
	switch (ins.op) {
		case 0: /* r_type */
			if (ins.funct == JR)
				return 0;
			return 1;
		case J:
		case SW:
		case SH:
		case SB: /* store memory */
		case BEQ:
		case BNE: /* branch */
		case HALT:
			return 0;
		default: /* i_type */
			return 1;
	}
}

word_t which_write_data(struct cpu_struct* cpu)
{
	if (is_load(cpu->pipeline[WB].ins)) {
		return cpu->pipeline[WB].read_data;
	} else {
		return cpu->pipeline[WB].alu_result;
	}
}

int is_load(struct ins_struct ins)
{
	switch (ins.op) {
		case LW:
		case LB:
		case LBU:
		case LH:
		case LHU:
			return 1;
		default:
			return 0;
	}
}

int is_branch(struct ins_struct ins)
{
	if (ins.op == BEQ)
		return 1;
	if (ins.op == BNE)
		return 1;
	return 0;
}

int is_jump(struct ins_struct ins)
{
	if (ins.op == 0 && ins.funct == JR)
		return 1;
	if (ins.op == J)
		return 1;
	if (ins.op == JAL)
		return 1;
	return 0;
}

