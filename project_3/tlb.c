#include <stdio.h>
#include "include/instruction.h"
#include "include/memory.h"

int check_tlb_hit(struct tlb_struct* tlb, word_t addr, int cycle)
{
	int i;
	int vpn = addr / tlb->page_size;
	for (i = 0; i < tlb->tlb_size; i++)
		if (tlb->entry[i].valid && tlb->entry[i].tag == vpn) {
			tlb->entry[i].last_cycle = cycle;
			/* printf("hit\n"); */
			return 1;
		}
	return 0;
}

int find_empty(struct tlb_struct* tlb, word_t addr, int cycle)
{
	int i;
	int vpn = addr / tlb->page_size;
	for (i = 0; i < tlb->tlb_size; i++)
		if (!tlb->entry[i].valid) {
			tlb->entry[i].valid = 1;
			tlb->entry[i].tag = vpn;
			tlb->entry[i].last_cycle = cycle;
			/* printf("empty\n"); */
			return 1;
		}
	return 0;
}

void find_lru(struct tlb_struct* tlb, word_t addr, int cycle)
{
	int i;
	int vpn = addr / tlb->page_size;
	int lru_index = -1;
	int lru_cycle = -1;
	for (i = 0; i < tlb->tlb_size; i++) {
		if (lru_cycle == -1 || lru_cycle > tlb->entry[i].last_cycle) {
			lru_index = i;
			lru_cycle = tlb->entry[i].last_cycle;
		}
	}
	tlb->entry[lru_index].tag = vpn;
	tlb->entry[lru_index].last_cycle = cycle;
	/* printf("lru %d\n", lru_index); */
}

int check_tlb(struct tlb_struct* tlb, word_t addr, int cycle)
{
	if (check_tlb_hit(tlb, addr, cycle))
		return 1;
	if (!find_empty(tlb, addr, cycle))
		find_lru(tlb, addr, cycle);
	return 0;
}

