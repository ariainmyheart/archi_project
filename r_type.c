#include "include/cpu.h"
#include "include/instruction.h"

void r_type(struct cpu_struct* cpu)
{
	struct ins_struct* ins = &cpu->current_ins;
	int s = cpu->reg[ins->rs];
	int t = cpu->reg[ins->rt];
	int d = ins->rd;
	int c = ins->shamt;
	switch (ins->funct) {
		case ADD:
			cpu->reg[d] = s + t;
			break;
		case SUB:
			cpu->reg[d] = s - t;
			break;
		case AND:
			cpu->reg[d] = s & t;
			break;
		case OR:
			cpu->reg[d] = s | t;
			break;
		case XOR:
			cpu->reg[d] = s ^ t;
			break;
		case NOR:
			cpu->reg[d] = ~(s | t);
			break;
		case NADD:
			cpu->reg[d] = ~(s & t);
			break;
		case SLT:
			cpu->reg[d] = (s < t) ? 1 : 0;
			break;
		case SLL:
			cpu->reg[d] = t << c;
			break;
		case SRL:
			cpu->reg[d] = cpu->reg[ins->rt] >> c;
			break;
		case SRA:
			cpu->reg[d] = t >> c;
			break;
		case JR:
			cpu->pc = s;
			break;
	}
}

