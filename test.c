#include <stdio.h>
#include <stdlib.h>
#include "include/instruction.h"
#include "include/cpu.h"

int main()
{
	struct cpu_struct* cpu = alloc_cpu();
	word_t ins;
	while (!is_halt(ins = fetch(cpu))) {
		decode(cpu, ins);
		execute(cpu);
	}
	free_cpu(cpu);
	return 0;
}
