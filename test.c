#include <stdio.h>
#include <stdlib.h>
#include "include/instruction.h"
#include "include/cpu.h"

int main()
{
	struct cpu_struct* cpu = alloc_cpu();
        cpu->current_ins = decode(cpu->ins[0]);
        printf("%x\n", cpu->current_ins->op);
        printf("%x\n", cpu->current_ins->rs);
        printf("%x\n", cpu->current_ins->rt);
        printf("%x\n", cpu->current_ins->imm);
	free_cpu(cpu);
	return 0;
}
