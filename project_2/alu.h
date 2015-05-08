#ifndef __ALU_H__
#define __ALU_H__

#include "type.h"
#include "cpu.h"

int alu_calculate(struct cpu_struct* cpu);
int r_type(struct cpu_struct* cpu);
int i_type(struct cpu_struct* cpu);
int j_type(struct cpu_struct* cpu);

#endif

