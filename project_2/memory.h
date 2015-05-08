#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "type.h"
#include "cpu.h"

word_t load_memory(struct cpu_struct* cpu, word_t addr, int byte, int* status);
void save_memory(struct cpu_struct* cpu, word_t value, word_t addr, int byte, int* status);

#endif

