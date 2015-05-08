#ifndef __CPU_H__
#define __CPU_H__

#include "type.h"

static const int IF = 0;
static const int ID = 1;
static const int EX = 2;
static const int DM = 3;
static const int WB = 4;

struct ins_struct {
	word_t hex;
	int op;
	int rs;
	int rt;
	int rd;
	int shamt;
	int funct;
	int imm;
	int immu;
	int addr;
	char name[10];
	word_t pc;
};

struct fwd_info {
	int has_fwd;
	int from;
	int to;
};

struct data_info {
	word_t value;
	int is_reg;
	int from_reg;
	char oprand;
	struct fwd_info fwd;
};

struct pipe_struct {
	struct ins_struct ins;
	struct data_info data1;
	struct data_info data2;
	struct data_info write_data;
	word_t alu_result;
	int write_reg;
	word_t read_data;
	int is_nop;
	int flush;
	int stall;
};

struct cpu_struct {
	struct pipe_struct pipeline[5];
	word_t pc;
	word_t last_pc;
	word_t reg[32];
	byte_t ins[1024];
	byte_t mem[1024];
};

static const char* stage_name[] = {
	"IF", "ID", "EX", "DM", "WB"
};

struct cpu_struct* init_cpu();
void free_cpu(struct cpu_struct* cpu);
int is_halt(struct cpu_struct* cpu);
void cpu_cycle(struct cpu_struct* cpu);
void cpu_next_cycle(struct cpu_struct* cpu);
void ins_fetch(struct cpu_struct* cpu);
void ins_decode(struct cpu_struct* cpu);
void execute(struct cpu_struct* cpu);
void data_mem(struct cpu_struct* cpu);
void write_back(struct cpu_struct* cpu);
void get_ins_name(struct ins_struct* ins, int is_nop);

void check_EX_DM_to_ID_fwd(struct cpu_struct* cpu);
void check_EX_DM_to_EX_fwd(struct cpu_struct* cpu);
void check_DM_WB_to_EX_fwd(struct cpu_struct* cpu);
void check_stall(struct cpu_struct* cpu);

#endif

