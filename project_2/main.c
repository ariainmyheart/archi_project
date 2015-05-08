#include <stdio.h>
#include "cpu.h"
#include "error.h"

static const char* stage_name[] = {
	"IF", "ID", "EX", "DM", "WB"
};

void register_snapshot(struct cpu_struct* cpu, int cycle, FILE* snap)
{
	fprintf(snap, "cycle %d\n", cycle);
	int i;
	for (i = 0; i < 32; i++) {
		fprintf(snap, "$%02d: 0x%08X\n", i, cpu->reg[i]);
	}
	fprintf(snap, "PC: 0x%08X\n", cpu->pc);
}

void fwd_snapshot(struct data_info* data, int stage, FILE* snap)
{
	if (data->fwd.has_fwd && data->fwd.to == stage) {
		fprintf(snap, " fwd_%s-%s_r%c_$%d", 
			stage_name[data->fwd.from-1],
			stage_name[data->fwd.from],
			data->oprand,
			data->from_reg);
	}
}

void pipeline_snapshot(struct cpu_struct* cpu, FILE* snap)
{
	fprintf(snap, "IF: 0x%08X", cpu->pipeline[IF].ins.hex);
	if (cpu->pipeline[ID].stall)
		fprintf(snap, " to_be_stalled");
	else if (cpu->pipeline[IF].flush) {
		fprintf(snap, " to_be_flushed");
	}
	fprintf(snap, "\n");

	fprintf(snap, "ID: %s", cpu->pipeline[ID].ins.name);
	if (cpu->pipeline[ID].stall)
		fprintf(snap, " to_be_stalled");
	fwd_snapshot(&cpu->pipeline[ID].data1, ID, snap);
	fwd_snapshot(&cpu->pipeline[ID].data2, ID, snap);
	fprintf(snap, "\n");

	fprintf(snap, "EX: %s", cpu->pipeline[EX].ins.name);
	fwd_snapshot(&cpu->pipeline[EX].data1, EX, snap);
	fwd_snapshot(&cpu->pipeline[EX].data2, EX, snap);
	fwd_snapshot(&cpu->pipeline[EX].write_data, EX, snap);
	fprintf(snap, "\n");

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
	int flag = 0;
	int status;
	while (!is_halt(cpu) && !flag) {
		register_snapshot(cpu, cycle++, snap);
		status = cpu_cycle(cpu);
		pipeline_snapshot(cpu, snap);
		cpu_next_cycle(cpu);
		flag = error_dump(err, cycle, status);
	}
	register_snapshot(cpu, cycle, snap);
	ins_decode(cpu);
	pipeline_snapshot(cpu, snap);
	free_cpu(cpu);
	fclose(snap);
	fclose(err);
	return 0;
}
