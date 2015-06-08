#include <stdio.h>
#include <assert.h>
#include "../include/readfile.h"
#include "../include/cpu.h"
#include "../include/instruction.h"
#include "../include/error.h"
#include "test.h"

void test_sign_extend()
{
	printf("===test sign extend===\n");
	int i;
	for (i = -5; i <= 5; i++) {
		word_t w = i & 0xf;
		int e = sign_extend(w, 4);		
		assert(i == e);
		printf("%d: pass\n", i);
	}
	printf("\n");
}

void test_fetch()
{
	printf("===test fetch===\n");
	struct cpu_struct cpu;
	cpu.pc = 0;
	cpu.ins[0] = 0x12;
	cpu.ins[1] = 0x34;
	cpu.ins[2] = 0x56;
	cpu.ins[3] = 0x78;
	assert(fetch(&cpu) == 0x12345678);
	printf("0x%08x\n\n", fetch(&cpu));
}

void test_decode()
{
	printf("===test decode===\n");
	struct cpu_struct cpu;
	decode(&cpu, 0x12345678);
	assert(cpu.current_ins.ins == 0x12345678);
	assert(cpu.current_ins.op == 4);
	assert(cpu.current_ins.rs == 17);
	assert(cpu.current_ins.rt == 20);
	assert(cpu.current_ins.rd == 10);
	assert(cpu.current_ins.shamt == 25);
	assert(cpu.current_ins.funct == 56);
	assert(cpu.current_ins.imm == 0x5678);
	assert(cpu.current_ins.immu == sign_extend(0x5678, 16));
	assert(cpu.current_ins.addr == 0x2345678);
	printf("pass\n\n");
}

