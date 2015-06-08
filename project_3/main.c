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
	cpu->tlb.page_size = 8;
	cpu->tlb.tlb_size = 1024 / 8 / 4;
	cpu->pte.page_size = 8;
	cpu->pte.tlb_size = 1024 / 8;
	word_t ins;
	int status, flag = 0;
	int cycle = 0;
	while (!is_halt(ins = fetch(cpu, cycle)) && !flag) {
		snapshot(cpu, cycle++, snap);
		decode(cpu, ins);
		status = execute(cpu);
		flag = error_dump(err, cycle, status);
	}
	if (!flag)
		snapshot(cpu, cycle++, snap);
	printf("tlb hit %d\n", cpu->tlb.hit);
	printf("tlb miss %d\n", cpu->tlb.miss);
	printf("pte hit %d\n", cpu->pte.hit);
	printf("pte miss %d\n", cpu->pte.miss);
	free_cpu(cpu);
	fclose(snap);
	fclose(err);
	return 0;
}
