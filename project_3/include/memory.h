#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "instruction.h"

struct entry_struct {
	int valid;
	int tag;
	int last_cycle;
};

struct tlb_struct {
	struct entry_struct entry[1024];
	int page_size;
	int tlb_size;
	int hit;
	int miss;
};

struct pte_struct {
	struct entry_struct entry[1024];
	int page_size;
	int ppn_size;
	int hit;
	int miss;
};

struct cache_struct {
	struct entry_struct entry[1024];
	int set_size;
	int associative;
	int block_size;
	int hit;
	int miss;
};

struct mem_struct {
	struct tlb_struct tlb;
	struct pte_struct pte;
	struct cache_struct cache;
};

void check_addr(struct mem_struct* mem, word_t addr, int cycle);

#endif

