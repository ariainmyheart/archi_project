#ifndef __CPU_H__
#define __CPU_H__

#include "instruction.h"

struct cpu_struct {
	word_t pc;
	word_t reg[32];
	byte_t ins[1024];
	byte_t mem[1024];
	struct ins_struct* current_ins;
};

struct cpu_struct* alloc_cpu();
void free_cpu(struct cpu_struct* cpu);
void free_cpu(struct cpu_struct* cpu);
word_t fetch(struct cpu_struct* cpu);
void decode(struct cpu_struct* cpu, word_t ins);
void execute(struct cpu_struct* cpu);

#endif
