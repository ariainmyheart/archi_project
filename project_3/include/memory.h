#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "instruction.h"

struct tlb_entry {
	int tag;
	int valid;
	int last_cycle;
};

struct tlb_struct {
	struct tlb_entry entry[1024];
	int page_size;
	int tlb_size;
	int hit;
	int miss;
};

struct pte_entry {
	int tag;
	int valid;
	int last_cycle;
};

struct pte_struct {
	struct pte_entry entry[1024];
	int page_size;
	int pte_size;
};

int check_tlb(struct tlb_struct* tlb, word_t addr, int cycle);
int check_pte(struct tlb_struct* tlb, word_t addr, int cycle);

#endif

