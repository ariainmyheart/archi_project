#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#define HALT 0x3f

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
	int addr;
};

int extract(word_t ins, int left, int right);
int is_halt(word_t ins);

#endif
