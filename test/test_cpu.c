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
