#include "include/cpu.h"
#include "include/instruction.h"

void i_type(struct cpu_struct* cpu)
{
	struct ins_struct* ins = cpu->current_ins;
	int s = cpu->reg[ins->rs];
	int t = ins->rt;
	int c = ins->imm;
	int cu = ins->immu;
	
	switch (ins->op) {
		case ADDI:
			cpu->reg[t] = s + c;
			break;
		case LW:
			cpu->reg[t] = 0;
			cpu->reg[t] |= cpu->mem[s+c] << 24;
			cpu->reg[t] |= cpu->mem[s+c+1] << 16;
			cpu->reg[t] |= cpu->mem[s+c+2] << 8;
			cpu->reg[t] |= cpu->mem[s+c+3];
			break;
		case LH:
			cpu->reg[t] = 0;
			cpu->reg[t] |= cpu->mem[s+c] << 8;
			cpu->reg[t] |= cpu->mem[s+c+1];
			(cpu->reg[t] << 16) >> 16;
			break;
		case LHU:
			cpu->reg[t] = 0;
			cpu->reg[t] |= cpu->mem[s+c] << 8;
			cpu->reg[t] |= cpu->mem[s+c+1];
			break;
		case LB:
			cpu->reg[t] = 0;
			cpu->reg[t] |= cpu->mem[s+c];
			(cpu->reg[t] << 24) >> 24;
			break;
		case LBU:
			cpu->reg[t] = 0;
			cpu->reg[t] |= cpu->mem[s+c];
			break;
		case SW:
			cpu->mem[s+c] = (cpu->reg[t] >> 24) & 0xff;
			cpu->mem[s+c+1] = (cpu->reg[t] >> 16) & 0xff;
			cpu->mem[s+c+2] = (cpu->reg[t] >> 8) & 0xff;
			cpu->mem[s+c+3] = cpu->reg[t] & 0xff;
			break;
		case SH:
			cpu->mem[s+c] = (cpu->reg[t] >> 8) & 0xff;
			cpu->mem[s+c+1] = cpu->reg[t] & 0xff;
			break;
		case SB:
			cpu->mem[s+c] = cpu->reg[t] & 0xff;
			break;
		case LUI:
			break;
		case ANDI:
			break;
		case ORI:
			break;
		case NORI:
			break;
		case SLTI:
			break;
		case BEQ:
			break;
		case BNE:
			break;
	}
}
