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
			addr = check_num_overflow(s, c, &status); //addr = s + c
			save_memory(cpu, cpu->reg[t], addr, 4, &status); //mem[addr] = cpu->reg[t]
			break;
		case SH:
			addr = check_num_overflow(s, c, &status); //addr = s + c
			save_memory(cpu, cpu->reg[t], addr, 2, &status); //mem[addr] = cpu->reg[t]
			break;
		case SB:
			addr = check_num_overflow(s, c, &status); //addr = s + c
			save_memory(cpu, cpu->reg[t], addr, 1, &status); //mem[addr] = cpu->reg[t]
			break;
		case LUI:
			write_register(cpu, t, cu << 16, &status);
			break;
		case ANDI:
			write_register(cpu, t, s & cu, &status);
			break;
		case ORI:
			write_register(cpu, t, s | cu, &status);
			break;
		case NORI:
			write_register(cpu, t, ~(s | cu), &status);
			break;
		case SLTI:
			write_register(cpu, t, s < c, &status);
			break;
		case BEQ:
			if (s == cpu->reg[t]) cpu->pc += 4*c;
			break;
		case BNE:
			if (s != cpu->reg[t]) cpu->pc += 4*c;
			break;
	}
}
