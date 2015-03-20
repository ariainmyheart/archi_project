#include "include/cpu.h"
#include "include/instruction.h"
#include "include/error.h"

void r_type(struct cpu_struct* cpu)
{
	struct ins_struct* ins = &cpu->current_ins;
	int s = cpu->reg[ins->rs];
	int t = cpu->reg[ins->rt];
	int d = ins->rd;
	int c = ins->shamt;
	word_t value;
	int status = 0;

	switch (ins->funct) {
		case ADD:
			value = check_num_overflow(s, t, &status);
			write_register(cpu, d, value, &status);
			break;
		case SUB:
			value = check_num_overflow(s, -t, &status);
			write_register(cpu, d, value, &status);
			break;
		case AND:
			value = s & t;
			write_register(cpu, d, value, &status);
			break;
		case OR:
			value = s | t;
			write_register(cpu, d, value, &status);
			break;
		case XOR:
			value = s ^ t;
			write_register(cpu, d, value, &status);
			break;
		case NOR:
			value = ~(s | t);
			write_register(cpu, d, value, &status);
			break;
		case NADD:
			value = ~(s & t);
			write_register(cpu, d, value, &status);
			break;
		case SLT:
			value = (s < t) ? 1 : 0;
			write_register(cpu, d, value, &status);
			break;
		case SLL:
			value = t << c;
			write_register(cpu, d, value, &status);
			break;
		case SRL:
			value = cpu->reg[ins->rt] >> c;
			write_register(cpu, d, value, &status);
			break;
		case SRA:
			value = t >> c;
			write_register(cpu, d, value, &status);
			break;
		case JR:
			cpu->pc = s;
			break;
	}
}

