#include <stdio.h>
#include <stdlib.h>
#include "include/instruction.h"

word_t get_word(FILE* in)
{
	unsigned char tmp;
	word_t ret = 0;
	int i;
	for (i = 0; i < 4; i++) {
		fread(&tmp, sizeof(unsigned char), 1, in);
		ret = ret << 8;
		ret = ret | tmp;
	}
	return ret;
}

word_t init_mem(word_t* ins, char option)
{
	FILE* file;
	if (option == 'i') file = fopen("iimage.bin", "rb");
	else file = fopen("dimage.bin", "rb");

	word_t init_addr = get_word(file);

	int n = get_word(file);

	int i;
	for (i = 0; i < n; i++)
		ins[i+init_addr] = get_word(file);

	fclose(file);

	return init_addr;
}
