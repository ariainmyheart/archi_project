#ifndef __READFILE_H__
#define __READFILE_H__

#include "instruction.h"

byte_t get_byte(FILE* in);
word_t get_word(FILE* in);
word_t init_mem(byte_t* ins, char option);

#endif

