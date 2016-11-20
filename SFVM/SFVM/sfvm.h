/* sfvm.h */
#ifndef SFVM_H

#define SFVM_H

#define TD_SIZE	8

extern struct SFVM sfvm;
enum CODE{									//指令码
	NOP, NEG, ADD, SUB, MUL, DIV, MOD, NOT,
	LESS, GRT, LESSE, GRTE, EQU, NEQU, AND, OR,
	BNOT, BAND, BOR, BXOR, BLSHF, BRSHF, BSRSHF, PUSH,
	PUSHP, PUSHV, PUSHT, COPYN, COPYRN, POP, STORE, STOREV,
	HALT, JMP, JTRUE, JFALSE, JTRUEPOP, JFALSEPOP, CALL, RET,
	LPUSHP, LPUSHV, LSTORE, LSTOREV, IN, INNUM, OUT, OUTNUM,
};

enum DEBUGCMD{
	CONTINUE,			//继续执行，直至下一个断点处
	FINISH,				//清除所有断点，全部执行
	LIST,				//列出当前指令
	DELETE,				//删除断点，参数是断点号或者监视内存地址
	BREAK,				//设置断点
	WWATCH,				//当内存被写入时中断
	RWATCH,				//当内存被读出时中断
	AWATCH,				//当内存被访问时中断
	INFO,				//列出所有中断点和监控内存
	PRINT,				//显示指定内存数据
	SYS,				//显示系统信息
	SYSD,				//显示系统详细信息
	UNKNOWN,			//未知命令
};

struct SFVM{
	unsigned int operation_data;		//保存操作数
	unsigned int PC;					//程序计数器
	unsigned char *program_base;		//程序区基址
	unsigned int program_szie;			//程序区大小
	unsigned int SP;					//栈指针
	unsigned int main_data_size;		//申请的总内存指针
	unsigned int *main_data_base;		//申请的总内存大小
	unsigned int stack_size;			//栈区大小
	unsigned int *stack_base;			//栈区指针
	unsigned int temp_data_size;		//临时数据区大小
	unsigned int *temp_data_base;		//临时数据区指针
	unsigned int temp_data_pointer;		//使用的临时数据区计数
	unsigned int data_size;				//数据区大小
	unsigned int *data_base;			//数据区指针
};

int sfvm_init(unsigned int stack_size, unsigned int temp_data_size, unsigned data_size, unsigned int program_size);

int sfvm_load(char *filename);

int sfvm_run();

int sfvm_debug(int cmd);

int setbreakpoint(unsigned int addr);

void vmerror(char *s);

#endif
