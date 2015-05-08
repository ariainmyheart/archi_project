#include "type.h"
#include "cpu.h"
#include "memory.h"
#include "error.h"

word_t load_memory(struct cpu_struct* cpu, word_t addr, int byte, int* status)
{
	word_t value = 0;
	int flag = 0;
	flag = check_addr_overflow(addr, byte, status) | check_data_misaligned(addr, byte, status);
	if (flag)
		return 0;

	int i;
	for (i = 0; i < byte; i++) {
		value <<= 8;
		value |= cpu->mem[addr+i];
	}
	return value;
}

void save_memory(struct cpu_struct* cpu, word_t value, word_t addr, int byte, int* status)
{
	int flag = 0;
	flag = check_addr_overflow(addr, byte, status) | check_data_misaligned(addr, byte, status);
	if (flag)
		return;

	int i;
	for (i = byte-1; i >= 0; i--) {
		cpu->mem[addr+i] = value & 0xff;
		value >>= 8;
	}
}

