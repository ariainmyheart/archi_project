#include "cpu.h"
#include "control.h"
#include "instruction.h"

word_t get_data_1(struct cpu_struct* cpu)
{
	int rs = cpu->pipeline[ID].ins.rs;
	int rt = cpu->pipeline[ID].ins.rt;
	int imm = cpu->pipeline[ID].ins.imm;
	switch (cpu->pipeline[ID].ins.op) {
		case 0: /* r_type */
			switch (cpu->pipeline[ID].ins.funct) {
				case SLL:
				case SRL:
				case SRA:
					return cpu->reg[rt];
				default:
					return cpu->reg[rs];
			}
		case 2:
		case 3: /* j_type */
			return 0;
		case LUI:
			return imm;
		default: /* i_type */
			return cpu->reg[rs];

	}
}

word_t get_data_2(struct cpu_struct* cpu)
{
	int rt = cpu->pipeline[ID].ins.rt;
	int shamt = cpu->pipeline[ID].ins.shamt;
	int immu = cpu->pipeline[ID].ins.immu;
	switch (cpu->pipeline[ID].ins.op) {
		case 0: /* r_type */
			switch (cpu->pipeline[ID].ins.funct) {
				case SLL:
				case SRL:
				case SRA:
					return shamt;
				default:
					return cpu->reg[rt];
			}
		case 2:
		case 3: /* j_type */
			return 0;
		case LUI:
			return 16;
		default: /* i_type */
			return immu;

	}
}

int get_write_reg(struct cpu_struct* cpu)
{
	int rt = cpu->pipeline[ID].ins.rt;
	int rd = cpu->pipeline[ID].ins.rd;
	switch (cpu->pipeline[ID].ins.op) {
		case 0: /* r_type */
			return rd;
		case 2:
		case 3: /* j_type */
			return 0;
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
		case 2:
		case 3: /* j_type */
			return 0;
		case SW:
		case SH:
		case SB: /* store memory */
			return 0;
		default: /* i_type */
			return 1;
	}
}

word_t which_write_data(struct cpu_struct* cpu)
{
	switch (cpu->pipeline[WB].ins.op) {
		case LW:
		case LH:
		case LHU:
		case LB:
		case LBU: /* load memory */
			return cpu->pipeline[WB].read_data;
		default: /* r_type i_type */
			return cpu->pipeline[WB].alu_result;
	}
}
