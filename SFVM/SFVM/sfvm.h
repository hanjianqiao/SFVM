/* sfvm.h */
#ifndef SFVM_H

#define SFVM_H

#define TD_SIZE	8

extern struct SFVM sfvm;
enum CODE{									//ָ����
	NOP, NEG, ADD, SUB, MUL, DIV, MOD, NOT,
	LESS, GRT, LESSE, GRTE, EQU, NEQU, AND, OR,
	BNOT, BAND, BOR, BXOR, BLSHF, BRSHF, BSRSHF, PUSH,
	PUSHP, PUSHV, PUSHT, COPYN, COPYRN, POP, STORE, STOREV,
	HALT, JMP, JTRUE, JFALSE, JTRUEPOP, JFALSEPOP, CALL, RET,
	LPUSHP, LPUSHV, LSTORE, LSTOREV, IN, INNUM, OUT, OUTNUM,
};

enum DEBUGCMD{
	CONTINUE,			//����ִ�У�ֱ����һ���ϵ㴦
	FINISH,				//������жϵ㣬ȫ��ִ��
	LIST,				//�г���ǰָ��
	DELETE,				//ɾ���ϵ㣬�����Ƕϵ�Ż��߼����ڴ��ַ
	BREAK,				//���öϵ�
	WWATCH,				//���ڴ汻д��ʱ�ж�
	RWATCH,				//���ڴ汻����ʱ�ж�
	AWATCH,				//���ڴ汻����ʱ�ж�
	INFO,				//�г������жϵ�ͼ���ڴ�
	PRINT,				//��ʾָ���ڴ�����
	SYS,				//��ʾϵͳ��Ϣ
	SYSD,				//��ʾϵͳ��ϸ��Ϣ
	UNKNOWN,			//δ֪����
};

struct SFVM{
	unsigned int operation_data;		//���������
	unsigned int PC;					//���������
	unsigned char *program_base;		//��������ַ
	unsigned int program_szie;			//��������С
	unsigned int SP;					//ջָ��
	unsigned int main_data_size;		//��������ڴ�ָ��
	unsigned int *main_data_base;		//��������ڴ��С
	unsigned int stack_size;			//ջ����С
	unsigned int *stack_base;			//ջ��ָ��
	unsigned int temp_data_size;		//��ʱ��������С
	unsigned int *temp_data_base;		//��ʱ������ָ��
	unsigned int temp_data_pointer;		//ʹ�õ���ʱ����������
	unsigned int data_size;				//��������С
	unsigned int *data_base;			//������ָ��
};

int sfvm_init(unsigned int stack_size, unsigned int temp_data_size, unsigned data_size, unsigned int program_size);

int sfvm_load(char *filename);

int sfvm_run();

int sfvm_debug(int cmd);

int setbreakpoint(unsigned int addr);

void vmerror(char *s);

#endif
