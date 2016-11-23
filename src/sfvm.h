/* sfvm.h */
#ifndef SFVM_H

#define SFVM_H

#define TD_SIZE	8

extern struct SFVM sfvm;
enum CODE{
	NOP = 0, NEG, ADD, SUB, MUL, DIV, MOD, NOT,
	LESS, GRT, LESSE, GRTE, EQU, NEQU, AND, OR,
	BNOT, BAND, BOR, BXOR, BLSHF, BRSHF, BSRSHF, PUSH,
	PUSHP, PUSHV, PUSHT, COPYN, COPYRN, POP, STORE, STOREV,
	HALT, JMP, JTRUE, JFALSE, JTRUEPOP, JFALSEPOP, CALL, RET,
	LPUSHP, LPUSHV, LSTORE, LSTOREV, IN, INNUM, OUT, OUTNUM,
};

enum DEBUGCMD{
	CONTINUE,			// continue until next break point
	FINISH,				// go to finish
	LIST,				// list temporary instruction
	DELETE,				// delete watch address or break point
	BREAK,				// set break point
	WWATCH,				// watch memory write
	RWATCH,				// watch memory read
	AWATCH,				// watch memory access
	INFO,				// list all break point and watch address
	PRINT,				// display specific address data
	SYS,				// display system information
	SYSD,				// display detailed system information
	UNKNOWN,
};

struct SFVM{
	unsigned int operation_data;
	unsigned int PC;
	unsigned char *program_base;
	unsigned int program_szie;
	unsigned int SP;
	unsigned int main_data_size;
	unsigned int *main_data_base;
	unsigned int stack_size;
	unsigned int *stack_base;
	unsigned int temp_data_size;
	unsigned int *temp_data_base;
	unsigned int temp_data_pointer;
	unsigned int data_size;
	unsigned int *data_base;
};

int sfvm_init(unsigned int stack_size, unsigned int temp_data_size, unsigned data_size, unsigned int program_size);

int sfvm_load(char *filename);

int sfvm_run();

int sfvm_debug(int cmd);

int setbreakpoint(unsigned int addr);

void vmerror(char *s);

#endif
