#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"

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

word_t get_ins(word_t** ins)
{
    FILE* iimage = fopen("iimage.bin", "rb");

    word_t init_pc = get_word(iimage);
    
    int n = get_word(iimage);
    
    *ins = (word_t*)malloc(sizeof(word_t)*n);

    int i;
    for (i = 0; i < n; i++)
        (*ins)[i] = get_word(iimage);
    
    fclose(iimage);

    return init_pc;
}
