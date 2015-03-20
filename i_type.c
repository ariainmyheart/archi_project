#include "include/cpu.h"
#include "include/instruction.h"
#include "include/error.h"

void i_type(struct cpu_struct* cpu)
{
	struct ins_struct* ins = &cpu->current_ins;
	int s = cpu->reg[ins->rs];
	int t = ins->rt;
	int c = ins->imm;
	int cu = ins->immu;
	word_t value, addr;
	int status = 0;
	
	switch (ins->op) {
		case ADDI:
			value = check_num_overflow(s, c, &status); //value = s + c
			write_register(cpu, t, value, &status); //reg[t] = value
			break;
		case LW:
			addr = check_num_overflow(s, c, &status); //addr = s + c
			value = load_memory(cpu, addr, 4, &status); //value = mem[addr]
			write_register(cpu, t, value, &status); //reg[t] = value
			break;
		case LH:
			addr = check_num_overflow(s, c, &status); //addr = s + c
			value = load_memory(cpu, addr, 2, &status); //value = mem[addr]
			value = sign_extend(value, 16);
			write_register(cpu, t, value, &status); //reg[t] = value
			break;
		case LHU:
			addr = check_num_overflow(s, c, &status); //addr = s + c
			value = load_memory(cpu, addr, 2, &status); //value = mem[addr]
			write_register(cpu, t, value, &status); //reg[t] = value
			break;
		case LB:
			addr = check_num_overflow(s, c, &status); //addr = s + c
			value = load_memory(cpu, addr, 1, &status); //value = mem[addr]
			value = sign_extend(value, 8);
			write_register(cpu, t, value, &status); //reg[t] = value
			break;
		case LBU:
			addr = check_num_overflow(s, c, &status); //addr = s + c
			value = load_memory(cpu, addr, 1, &status); //value = mem[addr]
			write_register(cpu, t, value, &status); //reg[t] = value
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
			cpu->reg[t] = cu << 16;
			break;
		case ANDI:
			cpu->reg[t] = s & cu;
			break;
		case ORI:
			cpu->reg[t] = s | cu;
			break;
		case NORI:
			cpu->reg[t] = ~(s | cu);
			break;
		case SLTI:
			cpu->reg[t] = s < c;
			break;
		case BEQ:
			if (s == cpu->reg[t]) cpu->pc += 4*c;
			break;
		case BNE:
			if (s != cpu->reg[t]) cpu->pc += 4*c;
			break;
	}
}
