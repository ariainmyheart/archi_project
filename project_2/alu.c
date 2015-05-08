#include "alu.h"
#include "cpu.h"
#include "instruction.h"
#include "type.h"
#include "error.h"

int alu_calculate(struct cpu_struct* cpu)
{
	switch (cpu->pipeline[EX].ins.op) {
		case 0:
			return r_type(cpu);
		case JAL:
			cpu->pipeline[EX].alu_result = cpu->pipeline[EX].ins.pc;
			return 0;
		default:
			return i_type(cpu);
	}
}

int r_type(struct cpu_struct* cpu)
{
	int status = 0;

	word_t data1 = cpu->pipeline[EX].data1.value;
	word_t data2 = cpu->pipeline[EX].data2.value;
	word_t* result = &cpu->pipeline[EX].alu_result;
	switch (cpu->pipeline[EX].ins.funct) {
		case ADD:
			*result = check_num_overflow(data1, data2, &status);
			/* *result = data1 + data2; */
			break;
		case SUB:
			*result = check_num_overflow(data1, -data2, &status);
			/* *result = data1 - data2; */
			break;
		case AND:
			*result = data1 & data2;
			break;
		case OR:
			*result = data1 | data2;
			break;
		case XOR:
			*result = data1 ^ data2;
			break;
		case NOR:
			*result = ~(data1 | data2);
			break;
		case NAND:
			*result = ~(data1 & data2);
			break;
		case SLT:
			*result = (int)data1 < (int)data2;
			break;
		case SLL:
			*result = data1 << data2;
			break;
		case SRL:
			*result = data1 >> data2;
			break;
		case SRA:
			*result = (int)data1 >> data2;
			break;
	}
	return status;
}

int i_type(struct cpu_struct* cpu)
{
	int status = 0;

	word_t data1 = cpu->pipeline[EX].data1.value;
	word_t data2 = cpu->pipeline[EX].data2.value;
	word_t* result = &cpu->pipeline[EX].alu_result;
	switch (cpu->pipeline[EX].ins.op) {
		case ADDI:
		case LW:
		case LH:
		case LHU:
		case LB:
		case LBU:
		case SW:
		case SH:
		case SB: /* addi, load, store */
			/* *result = data1 + data2; */
			*result = check_num_overflow(data1, data2, &status);
			break;
		case LUI:
			*result = data1 << data2;
			break;
		case ANDI:
			*result = data1 & data2;
			break;
		case ORI:
			*result = data1 | data2;
			break;
		case NORI:
			*result = ~(data1 | data2);
			break;
		case SLTI:
			*result =  (int)data1 < (int)data2;
	}
	return status;
}

