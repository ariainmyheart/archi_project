#include <stdio.h>
#include "include/instruction.h"
#include "include/cpu.h"
#include "include/error.h"

void snapshot(struct cpu_struct* cpu, int cycle, FILE* snap)
{
	fprintf(snap, "cycle %d\n", cycle);
	int i;
	for (i = 0; i < 32; i++) {
		fprintf(snap, "$%02d: 0x%08X\n", i, cpu->reg[i]);
	}
	fprintf(snap, "PC: 0x%08X\n", cpu->pc);
	fprintf(snap, "\n\n");
}

int main()
{
	FILE* snap = fopen("snapshot.rpt", "w");
	FILE* err = fopen("error_dump.rpt", "w");
	struct cpu_struct* cpu = alloc_cpu();
	word_t ins;
	int status, flag = 0;
	int cycle = 0;
	while (!is_halt(ins = fetch(cpu)) && !flag) {
		snapshot(cpu, cycle++, snap);
		decode(cpu, ins);
		status = execute(cpu);
		flag = error_dump(err, cycle, status);
	}
	if (!flag)
		snapshot(cpu, cycle++, snap);
	free_cpu(cpu);
	fclose(snap);
	fclose(err);
	return 0;
}
