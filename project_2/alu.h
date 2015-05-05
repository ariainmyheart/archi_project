#ifndef __ALU_H__
#define __ALU_H__

#include "type.h"
#include "cpu.h"

void alu_calculate(struct cpu_struct* cpu);
void r_type(struct cpu_struct* cpu);
void i_type(struct cpu_struct* cpu);
void j_type(struct cpu_struct* cpu);

#endif

