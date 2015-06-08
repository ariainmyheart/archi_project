#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "instruction.h"

struct tlb_entry {
	int tag, valid, last_cycle;
};

struct tlb_struct {
	struct tlb_entry entry[1024];
	int page_size;
	int tlb_size;
};

int check_tlb(struct tlb_struct* tlb, word_t addr, int cycle);

#endif

