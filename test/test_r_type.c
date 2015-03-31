#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../include/readfile.h"
#include "../include/cpu.h"
#include "../include/instruction.h"
#include "../include/error.h"
#include "test.h"

void test_add()
{
	printf("===test ADD===\n");
	struct cpu_struct cpu;
	cpu.current_ins.rs = 1;
	cpu.current_ins.rt = 2;
	cpu.current_ins.rd = 3;
	cpu.current_ins.funct = 0x20;
	int i;
	for (i = 0; i < 100; i++) {
		int a = rand();
		int b = rand();
		cpu.reg[1] = a;
		cpu.reg[2] = b;
		r_type(&cpu);
		printf("%d + %d: %d\n", a, b, cpu.reg[3]);
		assert(cpu.reg[3] == a+b);
	}
	printf("pass\n\n");
}

void test_slt()
{
	printf("===test SLT===\n");
	struct cpu_struct cpu;
	cpu.current_ins.rs = 1;
	cpu.current_ins.rt = 2;
	cpu.current_ins.rd = 3;
	cpu.current_ins.funct = 0x2a;
	int i;
	for (i = 0; i < 100; i++) {
		int a = rand();
		int b = rand();
		int c = (a < b) ? 1 : 0;
		cpu.reg[1] = a;
		cpu.reg[2] = b;
		r_type(&cpu);
		printf("%d < %d: %d\n", a, b, cpu.reg[3]);
		assert(cpu.reg[3] == c);
	}
	printf("pass\n\n");
}

void test_srl()
{
	printf("===test SRL===\n");
	struct cpu_struct cpu;
	cpu.current_ins.rt = 1;
	cpu.current_ins.rd = 2;
	cpu.current_ins.shamt = 3;
	cpu.current_ins.funct = 0x02;
	cpu.reg[1] = 0x80000000;
	r_type(&cpu);
	printf("%s\n", word_to_binary(cpu.reg[1], 0, 31));
	printf("%s\n", word_to_binary(cpu.reg[2], 0, 31));
	assert(cpu.reg[2] == 0x10000000);
	printf("pass\n\n");
}

void test_sra()
{
	printf("===test SRA===\n");
	struct cpu_struct cpu;
	cpu.current_ins.rt = 1;
	cpu.current_ins.rd = 2;
	cpu.current_ins.shamt = 3;
	cpu.current_ins.funct = 0x03;
	cpu.reg[1] = 0x80000000;
	r_type(&cpu);
	printf("%s\n", word_to_binary(cpu.reg[1], 0, 31));
	printf("%s\n", word_to_binary(cpu.reg[2], 0, 31));
	assert(cpu.reg[2] == 0xf0000000);
	printf("pass\n\n");
}

