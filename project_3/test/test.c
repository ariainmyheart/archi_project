#include <stdio.h>
#include "../include/readfile.h"
#include "../include/cpu.h"
#include "../include/instruction.h"
#include "../include/error.h"
#include "test.h"

char binary[33];
char* word_to_binary(word_t ins, int start, int end)
{
	int i;
	int n = 0;
	for (i = end; i >= start; i--) {
		binary[n++] = ((1u << i) & ins) ? '1' : '0';
	}
	binary[n] = '\0';
	return binary;
}

int main()
{
	test_num_overflow();
	return 0;
}
