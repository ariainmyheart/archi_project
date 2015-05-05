#ifndef __HAZARD_H__
#define __HAZARD_H__

#include "cpu.h"

int check_EX_DM_to_ID_fwd(struct cpu_struct* cpu);
int check_EX_DM_to_EX_fwd(struct cpu_struct* cpu);
int check_DM_WB_to_EX_fwd(struct cpu_struct* cpu);

#endif

