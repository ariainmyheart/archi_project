#ifndef __CPU_H__
#define __CPU_H__

#include "instruction.h"

struct cpu_struct {
	word_t pc;
	word_t reg[32];
	word_t* ins;
	word_t* mem;
	struct ins_struct* current_ins;
};

struct cpu_struct* alloc_cpu();
void free_cpu(struct cpu_struct* cpu);

#endif
