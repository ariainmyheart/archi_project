#include <stdlib.h>
#include <string.h>
#include "cpu.h"

struct cpu_struct* alloc_cpu()
{
	struct cpu_struct* cpu = (struct cpu_struct*)malloc(sizeof(struct cpu_struct));
	memset(cpu, 0, sizeof(struct cpu_struct));
	cpu->pc = get_ins(&cpu->ins);
	return cpu;
}

void free_cpu(struct cpu_struct* cpu)
{
	if (!cpu->ins)
		free(cpu->ins);
	if (!cpu->mem)
		free(cpu->mem);
	free(cpu);
}
