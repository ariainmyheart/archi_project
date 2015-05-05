#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "readfile.h"

byte_t get_byte(FILE* in)
{
	byte_t ret;
	fread(&ret, sizeof(byte_t), 1, in);
	return ret;
}

word_t get_word(FILE* in)
{
	byte_t tmp;
	word_t ret = 0;
	int i;
	for (i = 0; i < 4; i++) {
		tmp = get_byte(in);
		ret = ret << 8;
		ret = ret | tmp;
	}
	return ret;
}

word_t init_byte_array(byte_t* array, char option)
{
	FILE* file;
	if (option == 'i') file = fopen("iimage.bin", "rb");
	else file = fopen("dimage.bin", "rb");

	word_t init_addr = get_word(file);

	int n = get_word(file);

	int i;
	for (i = 0; i < n*4; i++) {
		if (option == 'i') array[i+init_addr] = get_byte(file);
		else array[i] = get_byte(file);
	}

	fclose(file);

	return init_addr;
}

