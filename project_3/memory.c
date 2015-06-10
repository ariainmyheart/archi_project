#include <stdio.h>
#include "include/instruction.h"
#include "include/memory.h"

int check_hit(struct entry_struct* row, int size, int tag, int cycle)
{
	int i;
	for (i = 0; i < size; i++) {
		if (row[i].valid && row[i].tag == tag) {
			row[i].last_cycle = cycle;
			/* printf("hit %d\n", i); */
			return i;
		}
	}
	/* printf("miss\n"); */
	return -1; }

int find_empty(struct entry_struct* row, int size, int tag, int cycle)
{
	int i;
	for (i = 0; i < size; i++) {
		if (!row[i].valid) {
			row[i].valid = 1;
			row[i].tag = tag;
			row[i].last_cycle = cycle;
			/* printf("empty %d\n", i); */
			return i;
		}
	}
	/* printf("not empty\n"); */
	return -1;
}

int find_lru(struct entry_struct* row, int size, int tag, int cycle, int* kicked_out)
{
	int i;
	int lru_index = -1;
	int lru_cycle = -1;
	for (i = 0; i < size; i++) {
		if (lru_cycle == -1 || lru_cycle > row[i].last_cycle) {
			lru_index = i;
			lru_cycle = row[i].last_cycle;
		}
	}
	if (kicked_out)
		*kicked_out = row[lru_index].tag;
	row[lru_index].tag = tag;
	row[lru_index].last_cycle = cycle;
	/* printf("lru %d\n", lru_index); */
	return lru_index;
}

int check_tlb(struct tlb_struct* tlb, word_t addr, int cycle)
{
	/* printf("tlb\n"); */
	int tag = addr / tlb->page_size;
	if (check_hit(tlb->entry, tlb->tlb_size, tag, cycle) != -1) {
		tlb->hit++;
		return 1;
	}
	tlb->miss++;
	if (find_empty(tlb->entry, tlb->tlb_size, tag, cycle) == -1)
		find_lru(tlb->entry, tlb->tlb_size, tag, cycle, NULL);
	return 0;
}

int check_pte(struct pte_struct* pte, word_t addr, int cycle, word_t* paddr, int* swap_ppn, int* swap_vpn, int* has_swap)
{
	/* printf("pte\n"); */
	int ppn;
	int vpn = addr / pte->page_size;
	int offset = addr % pte->page_size;
	if ((ppn =check_hit(pte->entry, pte->ppn_size, vpn, cycle)) != -1) {
		*paddr = ppn * pte->page_size + offset;
		return 1;
	}
	if ((ppn = find_empty(pte->entry, pte->ppn_size, vpn, cycle)) == -1) {
		ppn = find_lru(pte->entry, pte->ppn_size, vpn, cycle, swap_vpn);
		*has_swap = 1;
	}
	*paddr = ppn * pte->page_size + offset;
	*swap_ppn = ppn;
	return 0;
}

void page_fault(struct cache_struct* cache, int swap_ppn, struct tlb_struct* tlb, int swap_vpn, int page_size)
{
	int i, j;
	for (i = 0; i < cache->set_size; i++) {
		for (j = 0; j < cache->associative; j++) {
			int x = i * cache->associative + j;
			int tag = cache->entry[x].tag;
			int paddr = tag * cache->set_size + i;
			int ppn = paddr / page_size;
			if (ppn == swap_ppn) {
				cache->entry[x].valid = 0;
				/* printf("pf %d\n", ppn); */
			}
		}
	}
	for (i = 0; i < tlb->tlb_size; i++)
		if (tlb->entry[i].valid && tlb->entry[i].tag == swap_vpn) {
			tlb->entry[i].valid = 0;
			/* printf("pf tlb %d\n", swap_vpn); */
		}
}

int check_cache(struct cache_struct* cache, word_t paddr, int cycle)
{
	/* printf("cache\n"); */
	int paddr_b = paddr / cache->block_size;
	int tag = paddr_b / cache->set_size;
	int index = paddr_b % cache->set_size * cache->associative;
	if (check_hit(cache->entry+index, cache->associative, tag, cycle) != -1) {
		cache->hit++;
		return 1;
	}
	cache->miss++;
	if (find_empty(cache->entry+index, cache->associative, tag, cycle) == -1) {
		find_lru(cache->entry+index, cache->associative, tag, cycle, NULL);
	}
	return 0;
}

void check_addr(struct mem_struct* mem, word_t addr, int cycle)
{
	word_t paddr;
	int has_swap = 0;
	int swap_ppn, swap_vpn;
	if (check_tlb(&mem->tlb, addr, cycle)) {
		check_pte(&mem->pte, addr, cycle, &paddr, &swap_ppn, &swap_vpn, &has_swap);
	} else {
		if (check_pte(&mem->pte, addr, cycle, &paddr, &swap_ppn, &swap_vpn, &has_swap)) {
			mem->pte.hit++;
		} else {
			mem->pte.miss++;
			if (has_swap) {
				page_fault(&mem->cache, swap_ppn, &mem->tlb, swap_vpn, mem->pte.page_size);
			}
		}
	}
	/* printf("paddr %d\n", paddr); */
	check_cache(&mem->cache, paddr, cycle);
}

