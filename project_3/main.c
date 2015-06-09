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

	cpu->i_mem.tlb.page_size = 8;
	cpu->i_mem.tlb.tlb_size = 1024 / 8 / 4;

	cpu->i_mem.pte.page_size = 8;
	cpu->i_mem.pte.ppn_size = 64 / 8;

	cpu->i_mem.cache.set_size = 4;
	cpu->i_mem.cache.associative = 4;
	cpu->i_mem.cache.block_size = 4;

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
	printf("tlb hit %d\n", cpu->i_mem.tlb.hit);
	printf("tlb miss %d\n", cpu->i_mem.tlb.miss);
	printf("pte hit %d\n", cpu->i_mem.pte.hit);
	printf("pte miss %d\n", cpu->i_mem.pte.miss);
	printf("cache hit %d\n", cpu->i_mem.cache.hit);
	printf("cache miss %d\n", cpu->i_mem.cache.miss);
	free_cpu(cpu);
	fclose(snap);
	fclose(err);
	return 0;
}
