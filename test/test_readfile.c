#include <stdio.h>
#include <assert.h>
#include "../include/readfile.h"
#include "../include/cpu.h"
#include "../include/instruction.h"
#include "../include/error.h"
#include "test.h"

void test_get_byte()
{
	printf("===test get byte ===\n");
	FILE* file = fopen("test/test.bin", "rb");
	byte_t byte = get_byte(file);
	assert(byte == '1');
	printf("%s\n", word_to_binary(byte, 0, 7));
	fclose(file);
	printf("\n");
}

void test_get_word()
{}

void test_init_mem()
{}
