#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#define HALT 0xffffffff

typedef unsigned int word_t;

struct ins_struct {
	word_t ins;
	int op;
	int rs;
	int rt;
	int rd;
	int shamt;
	int funct;
	int imm;
};

#endif
