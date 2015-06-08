#ifndef __CPU_H__
#define __CPU_H__

#include "instruction.h"
#include "memory.h"

struct cpu_struct {
	word_t pc;
	word_t reg[32];
	byte_t ins[1024];
	byte_t mem[1024];
	struct ins_struct current_ins;
	struct tlb_struct tlb;
	struct tlb_struct pte;
};

int sign_extend(int num, int len);
struct cpu_struct* alloc_cpu();
void free_cpu(struct cpu_struct* cpu);
void free_cpu(struct cpu_struct* cpu);
word_t fetch(struct cpu_struct* cpu, int cycle);
void decode(struct cpu_struct* cpu, word_t ins);
int execute(struct cpu_struct* cpu);
int i_type(struct cpu_struct* cpu);
int j_type(struct cpu_struct* cpu);
int r_type(struct cpu_struct* cpu);
word_t load_memory(struct cpu_struct* cpu, word_t addr, int byte, int* status);
void save_memory(struct cpu_struct* cpu, word_t value, word_t addr, int byte, int* status);
void write_register(struct cpu_struct* cpu, int num, word_t value, int* status, int is_nop);

#endif
