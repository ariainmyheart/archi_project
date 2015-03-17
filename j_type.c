#include "include/cpu.h"
#include "include/instruction.h"

void j_type(struct cpu_struct* cpu)
{
	struct ins_struct* ins = &cpu->current_ins;
	switch (ins->op) {
		case JAL:
			cpu->reg[31] = cpu->pc;
		case J:
			cpu->pc = (cpu->pc & (0xf<<28)) | (ins->addr<<2);
			break;
	}
}
