#ifndef __READFILE_H__
#define __READFILE_H__

#include "type.h"

byte_t get_byte(FILE* in);
word_t get_word(FILE* in);
word_t init_byte_array(byte_t* array, char option);

#endif

