#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#define HALT 0x3f

#define ADD 0x20
#define SUB 0x22
#define AND 0x24
#define OR 0x25
#define XOR 0x26
#define NOR 0x27
#define NADD 0x28
#define SLT 0x2A
#define SLL 0x00
#define SRL 0x02
#define SRA 0x03
#define JR 0x08

#define ADDI 0x08
#define LW 0x23
#define LH 0x21
#define LHU 0x25
#define LB 0x20
#define LBU 0x24
#define SW 0x2b
#define SH 0x29
#define SB 0x28
#define LUI 0x0f
#define ANDI 0x0c
#define ORI 0x0d
#define NORI 0x0e
#define SLTI 0x0a
#define BEQ 0x04
#define BNE 0x05

#define J 0x02
#define JAL 0x03

typedef unsigned char byte_t;
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
	int immu;
	int addr;
};

int extract(word_t ins, int left, int right);
int is_halt(word_t ins);
void print_ins(struct ins_struct* ins);

#endif
