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

void report(struct cpu_struct* cpu, FILE* rpt)
{
	fprintf(rpt, "ICache :\n");
	fprintf(rpt, "# hits: %u\n", cpu->i_mem.cache.hit);
	fprintf(rpt, "# misses: %u\n\n", cpu->i_mem.cache.miss);
	fprintf(rpt, "DCache :\n");
	fprintf(rpt, "# hits: %u\n", cpu->d_mem.cache.hit);
	fprintf(rpt, "# misses: %u\n\n", cpu->d_mem.cache.miss);
	fprintf(rpt, "ITLB :\n");
	fprintf(rpt, "# hits: %u\n", cpu->i_mem.tlb.hit);
	fprintf(rpt, "# misses: %u\n\n", cpu->i_mem.tlb.miss);
	fprintf(rpt, "DTLB :\n");
	fprintf(rpt, "# hits: %u\n", cpu->d_mem.tlb.hit);
	fprintf(rpt, "# misses: %u\n\n", cpu->d_mem.tlb.miss);
	fprintf(rpt, "IPageTable :\n");
	fprintf(rpt, "# hits: %u\n", cpu->i_mem.pte.hit);
	fprintf(rpt, "# misses: %u\n\n", cpu->i_mem.pte.miss);
	fprintf(rpt, "DPageTable :\n");
	fprintf(rpt, "# hits: %u\n", cpu->d_mem.pte.hit);
	fprintf(rpt, "# misses: %u\n\n", cpu->d_mem.pte.miss);
}

int main()
{
	FILE* snap = fopen("snapshot.rpt", "w");
	FILE* err = fopen("error_dump.rpt", "w");
	FILE* rpt = fopen("report.rpt", "w");
	struct cpu_struct* cpu = alloc_cpu();

	cpu->i_mem.tlb.page_size = 8;
	cpu->i_mem.tlb.tlb_size = 1024 / 8 / 4;

	cpu->i_mem.pte.page_size = 8;
	cpu->i_mem.pte.ppn_size = 64 / 8;

	cpu->i_mem.cache.set_size = 4;
	cpu->i_mem.cache.associative = 4;
	cpu->i_mem.cache.block_size = 4;

	cpu->d_mem.tlb.page_size = 16;
	cpu->d_mem.tlb.tlb_size = 1024 / 16 / 4;

	cpu->d_mem.pte.page_size = 16;
	cpu->d_mem.pte.ppn_size = 32 / 16;

	cpu->d_mem.cache.set_size = 4;
	cpu->d_mem.cache.associative = 1;
	cpu->d_mem.cache.block_size = 4;

	word_t ins;
	int status, flag = 0;
	while (!is_halt(ins = fetch(cpu)) && !flag) {
		snapshot(cpu, cpu->cycle++, snap);
		decode(cpu, ins);
		status = execute(cpu);
		flag = error_dump(err, cpu->cycle, status);
	}
	if (!flag)
		snapshot(cpu, cpu->cycle++, snap);
	report(cpu, rpt);
	free_cpu(cpu);
	fclose(snap);
	fclose(err);
	return 0;
}
