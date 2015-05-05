#include "alu.h"
#include "cpu.h"
#include "instruction.h"
#include "type.h"

void alu_calculate(struct cpu_struct* cpu)
{
	switch (cpu->pipeline[EX].ins.op) {
		case 0:
			r_type(cpu);
			break;
		case 2:
		case 3:
			/* j_type(cpu); */
			break;
		default:
			i_type(cpu);
			break;
	}
}

void r_type(struct cpu_struct* cpu)
{
	word_t data1 = cpu->pipeline[EX].data1;
	word_t data2 = cpu->pipeline[EX].data2;
	word_t* result = &cpu->pipeline[EX].alu_result;
	switch (cpu->pipeline[EX].ins.funct) {
		case ADD:
			*result = data1 + data2;
			break;
		case SUB:
			*result = data1 - data2;
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
		case NADD:
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
}

void i_type(struct cpu_struct* cpu)
{
	word_t data1 = cpu->pipeline[EX].data1;
	word_t data2 = cpu->pipeline[EX].data2;
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
			*result = data1 + data2;
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
}

