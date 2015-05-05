#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "type.h"
#include "cpu.h"

word_t get_data_1(struct cpu_struct* cpu);
word_t get_data_2(struct cpu_struct* cpu);
int get_write_reg(struct cpu_struct* cpu);
int has_write_reg(struct ins_struct ins);
word_t which_write_data(struct cpu_struct* cpu);

#endif 

