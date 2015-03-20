#ifndef __CPU_H__
#define __CPU_H__

#include "instruction.h"

struct cpu_struct {
	word_t pc;
	word_t reg[32];
	byte_t ins[1024];
	byte_t mem[1024];
	struct ins_struct current_ins;
};

int sign_extend(int num, int len);
struct cpu_struct* alloc_cpu();
void free_cpu(struct cpu_struct* cpu);
void free_cpu(struct cpu_struct* cpu);
word_t fetch(struct cpu_struct* cpu);
void decode(struct cpu_struct* cpu, word_t ins);
void execute(struct cpu_struct* cpu);
void i_type(struct cpu_struct* cpu);
void j_type(struct cpu_struct* cpu);
void r_type(struct cpu_struct* cpu);
word_t load_memory(struct cpu_struct* cpu, word_t addr, int byte, int* status);
void write_register(struct cpu_struct* cpu, int num, word_t value, int* status);

#endif
