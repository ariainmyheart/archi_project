#include <stdio.h>
#include "cpu.h"

void register_snapshot(struct cpu_struct* cpu, int cycle, FILE* snap)
{
	fprintf(snap, "cycle %d\n", cycle);
	int i;
	for (i = 0; i < 32; i++) {
		fprintf(snap, "$%02d: 0x%08X\n", i, cpu->reg[i]);
	}
	fprintf(snap, "PC: 0x%08X\n", cpu->pc);
}

void pipeline_snapshot(struct cpu_struct* cpu, FILE* snap)
{
	fprintf(snap, "IF: 0x%08X\n", cpu->pipeline[IF].ins.hex);
	fprintf(snap, "ID: %s\n", cpu->pipeline[ID].ins.name);
	fprintf(snap, "EX: %s\n", cpu->pipeline[EX].ins.name);
	fprintf(snap, "DM: %s\n", cpu->pipeline[DM].ins.name);
	fprintf(snap, "WB: %s\n", cpu->pipeline[WB].ins.name);
	fprintf(snap, "\n\n");
}

int main()
{
	FILE* snap = fopen("snapshot.rpt", "w");
	FILE* err = fopen("error_dump.rpt", "w");
	struct cpu_struct* cpu = init_cpu();
	int cycle = 0;
	while (!is_halt(cpu)) {
		register_snapshot(cpu, cycle++, snap);
		cpu_cycle(cpu);
		pipeline_snapshot(cpu, snap);
		cpu_next_cycle(cpu);
	}
	register_snapshot(cpu, cycle, snap);
	ins_decode(cpu);
	pipeline_snapshot(cpu, snap);
	free_cpu(cpu);
	fclose(snap);
	fclose(err);
	return 0;
}
