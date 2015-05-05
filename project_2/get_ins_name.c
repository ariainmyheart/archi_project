#include <string.h>
#include "cpu.h"
#include "instruction.h"

void r_type_name(struct ins_struct* ins);
void i_type_name(struct ins_struct* ins);
void j_type_name(struct ins_struct* ins);

void get_ins_name(struct ins_struct* ins, int is_nop)
{
	if (is_nop) {
		strcpy(ins->name, "NOP");
		return;
	}
	switch (ins->op) {
		case 0:
			r_type_name(ins); break;
		case 2:
			strcpy(ins->name, "J"); break;
		case 3:
			strcpy(ins->name, "JAL"); break;
		case HALT:
			strcpy(ins->name, "HALT"); break;
		default:
			i_type_name(ins); break;
	}
}

void r_type_name(struct ins_struct* ins)
{
	switch (ins->funct) {
		case ADD: strcpy(ins->name, "ADD"); break;
		case SUB: strcpy(ins->name, "SUB"); break;
		case AND: strcpy(ins->name, "AND"); break;
		case OR: strcpy(ins->name, "OR"); break;
		case XOR: strcpy(ins->name, "XOR"); break;
		case NOR: strcpy(ins->name, "NOR"); break;
		case NADD: strcpy(ins->name, "NADD"); break;
		case SLT: strcpy(ins->name, "SLT"); break;
		case SLL: strcpy(ins->name, "SLL"); break;
		case SRL: strcpy(ins->name, "SRL"); break;
		case SRA: strcpy(ins->name, "SRA"); break;
		case JR: strcpy(ins->name, "JR"); break;
	}
}

void i_type_name(struct ins_struct* ins)
{
	switch (ins->op) {
		case ADDI: strcpy(ins->name, "ADDI"); break;
		case LW: strcpy(ins->name, "LW"); break;
		case LH: strcpy(ins->name, "LH"); break;
		case LHU: strcpy(ins->name, "LHU"); break;
		case LB: strcpy(ins->name, "LB"); break;
		case LBU: strcpy(ins->name, "LBU"); break;
		case SW: strcpy(ins->name, "SW"); break;
		case SH: strcpy(ins->name, "SH"); break;
		case SB: strcpy(ins->name, "SB"); break;
		case LUI: strcpy(ins->name, "LUI"); break;
		case ANDI: strcpy(ins->name, "ANDI"); break;
		case ORI: strcpy(ins->name, "ORI"); break;
		case NORI: strcpy(ins->name, "NORI"); break;
		case SLTI: strcpy(ins->name, "SLTI"); break;
		case BEQ: strcpy(ins->name, "BEQ"); break;
		case BNE: strcpy(ins->name, "BNE"); break;
	}
}

