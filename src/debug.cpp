#include "sfvm.h"
#include "string.h"
#include <iostream>

#define	MAXBREAKPOINT	100
#define	MAXWATCHEDADDR	100

struct WATCHPOINT{
	unsigned char type;
	unsigned int addr;
};

unsigned int *breakpoints;
WATCHPOINT *watchedaddr;

void vmerror(const char *s){
	std::cout << s << std::endl;
	exit(1);
}

int setbreakpoint(unsigned int addr){
	for (int i = 0; i < MAXBREAKPOINT; i++){
		if (breakpoints[i] == 0xffffffff){
			breakpoints[i] = addr;
			return 0;
		}
	}
	return -1;
}

int lookup(unsigned int target, unsigned int *arr, unsigned int size){
	for (unsigned int i = 0; i < size; i++){
		if (arr[i] == target){
			return i;
		}
	}
	return -1;
}

int setwatchpoint(unsigned int addr, unsigned char type){
	for (int i = 0; i < MAXWATCHEDADDR; i++){
		if (watchedaddr[i].type == 0x00){
			watchedaddr[i].addr = addr;
			watchedaddr[i].type = type;
			return 0;
		}
	}
	return -1;
}

unsigned char lookup(unsigned int target, WATCHPOINT *arr, unsigned int size){
	for (unsigned int i = 0; i < size; i++){
		if (arr[i].addr == target){
			return arr[i].type;
		}
	}
	return 0;
}


void debug(){
	char cmdstr[64];
	int cmd;
	int i;
	bool loop = true;
	std::cout << "Entered debug breakpoint.\n";
	while (loop){
		std::cin >> cmdstr;
		if (stringcmp(cmdstr, "continue") == 0){
			cmd = DEBUGCMD::CONTINUE;
		}
		else if (stringcmp(cmdstr, "finish") == 0){
			cmd = DEBUGCMD::FINISH;
		}
		else if (stringcmp(cmdstr, "list") == 0){
			cmd = DEBUGCMD::LIST;
		}
		else if (stringcmp(cmdstr, "delete") == 0){
			cmd = DEBUGCMD::DELETE;
		}
		else if (stringcmp(cmdstr, "break") == 0){
			cmd = DEBUGCMD::BREAK;
		}
		else if (stringcmp(cmdstr, "wwatch") == 0){
			cmd = DEBUGCMD::WWATCH;
		}
		else if (stringcmp(cmdstr, "rwatch") == 0){
			cmd = DEBUGCMD::RWATCH;
		}
		else if (stringcmp(cmdstr, "awatch") == 0){
			cmd = DEBUGCMD::AWATCH;
		}
		else if (stringcmp(cmdstr, "info") == 0){
			cmd = DEBUGCMD::INFO;
		}
		else if (stringcmp(cmdstr, "print") == 0){
			cmd = DEBUGCMD::PRINT;
		}
		else if (stringcmp(cmdstr, "sys") == 0){
			cmd = DEBUGCMD::SYS;
		}
		else if (stringcmp(cmdstr, "sysd") == 0){
			cmd = DEBUGCMD::SYSD;
		}
		else{
			cmd = DEBUGCMD::UNKNOWN;
		}

		switch (cmd){
		case DEBUGCMD::CONTINUE:
			loop = false;
			std::cout << '\n';
			break;
		case DEBUGCMD::FINISH:
			for (i = 0; i < MAXBREAKPOINT; i++){
				breakpoints[i] = 0xffffffff;
			}
			for (i = 0; i < MAXWATCHEDADDR; i++){
				watchedaddr[i].addr = 0xffffffff;
				watchedaddr[i].type = 0x00;
			}
			loop = false;
			std::cout << '\n';
			break;
		case DEBUGCMD::LIST:
			std::cout << (int)sfvm.program_base[sfvm.PC] << '\n';
			std::cout << '\n';
			break;
		case DEBUGCMD::DELETE:
			std::cin >> cmdstr;
			if (stringcmp(cmdstr, "pc") == 0){
				std::cin >> i;
				i = lookup(i, breakpoints, MAXBREAKPOINT);
				breakpoints[i] = 0xffffffff;
			}
			else if (stringcmp(cmdstr, "addr") == 0){
				std::cin >> i;
				for (unsigned int i = 0; i < MAXWATCHEDADDR; i++){
					if (watchedaddr[i].addr == i){
						watchedaddr[i].type = 0x00;
						watchedaddr[i].addr = 0xffffffff;
						break;
					}
				}
			}
			std::cout << '\n';
			break;
		case DEBUGCMD::BREAK:
			std::cin >> i;
			setbreakpoint(i);
			std::cout << '\n';
			break;
		case DEBUGCMD::WWATCH:
			std::cin >> i;
			setwatchpoint(i, 0x02);
			std::cout << '\n';
			break;
		case DEBUGCMD::RWATCH:
			std::cin >> i;
			setwatchpoint(i, 0x01);
			std::cout << '\n';
			break;
		case DEBUGCMD::AWATCH:
			std::cin >> i;
			setwatchpoint(i, 0x04);
			std::cout << '\n';
			break;
		case DEBUGCMD::INFO:
			for (i = 0; i < MAXBREAKPOINT; i++){
				if (breakpoints[i] != 0xffffffff){
					std::cout << "Break point at: " << breakpoints[i] << '\n';
				}
			}
			for (i = 0; i < MAXWATCHEDADDR; i++){
				if (watchedaddr[i].type != 0){
					std::cout << "Memory watched at: " << watchedaddr[i].addr << " on ";
					if (watchedaddr[i].type == 0x01){
						std::cout << "Read.\n";
					}
					else if (watchedaddr[i].type == 0x02){
						std::cout << "Write.\n";
					}
					else if (watchedaddr[i].type == 0x04){
						std::cout << "Read and Write.\n";
					}
				}
			}
			std::cout << '\n';
			break;
		case DEBUGCMD::PRINT:
			std::cin >> i;
			std::cout << sfvm.data_base[i] << '\n';
			std::cout << '\n';
			break;
		case DEBUGCMD::SYS:
			std::cout << "PC: " << sfvm.PC << "\tSP: " << sfvm.SP << '\n';
			std::cout << '\n';
			break;
		case DEBUGCMD::SYSD:
			std::cout << "PC: " << sfvm.PC << "\tSP: " << sfvm.SP << '\n';
			std::cout << "Data memory: " << sfvm.data_size << '\n';
			std::cout << "Stack size: " << sfvm.stack_size << '\n';
			std::cout << "Temp data size: " << TD_SIZE << '\n';
			std::cout << '\n';
			break;
		case DEBUGCMD::UNKNOWN:
			std::cout << "Unknown command...\n";
			std::cout << '\n';
			break;
		default:
			std::cout << "Bad command...\n";
			std::cout << '\n';
		}
	}
}

int sfvm_debug(int cmd){
	int code = 0;
	int get_i;
	wchar_t get_c;
	int i;
	breakpoints = new unsigned int[MAXBREAKPOINT];
	for (i = 0; i < MAXBREAKPOINT; i++){
		breakpoints[i] = 0xffffffff;
	}
	watchedaddr = new WATCHPOINT[MAXWATCHEDADDR];
	for (i = 0; i < MAXWATCHEDADDR; i++){
		watchedaddr[i].addr = 0xffffffff;
		watchedaddr[i].type = 0x00;
	}

	sfvm.PC = 0;
	if (cmd != -1){
		setbreakpoint(cmd);
	}

	while (true){
		if (lookup(sfvm.PC, breakpoints, MAXBREAKPOINT) != -1){
			debug();
		}
		code = sfvm.program_base[sfvm.PC++];
		if (code & 0x80){
			sfvm.operation_data = 0x00000000;
			for (int i = 0; i < 4; i++){
				sfvm.operation_data |= (sfvm.program_base[sfvm.PC++] << (i * 8));
			}
			if ((sfvm.operation_data & 0x7fffffff) == 0x7fffffff){
				printf("Invalid operation data at: %d\n", sfvm.PC - 4);
				return 1;
			}
		}
		switch (code & 0x7f){
		case CODE::NOP:
			break;
		case CODE::NEG:
			sfvm.stack_base[sfvm.SP] = -((signed int)sfvm.stack_base[sfvm.SP]);
			break;
		case CODE::ADD:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] + sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::SUB:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] - sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::MUL:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] * sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::DIV:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			if (sfvm.stack_base[sfvm.SP] == 0){
				vmerror("Dividing ZERO error.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] / sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::MOD:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			if (sfvm.stack_base[sfvm.SP] == 0){
				vmerror("MOD ZERO error.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] % sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::NOT:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP] = !sfvm.stack_base[sfvm.SP];
			break;
		case CODE::LESS:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] < sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::GRT:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] > sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::LESSE:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] <= sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::GRTE:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] >= sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::EQU:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] == sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::NEQU:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] != sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::AND:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] && sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::OR:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] || sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BNOT:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP] = ~sfvm.stack_base[sfvm.SP];
			break;
		case CODE::BAND:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] & sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BOR:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] | sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BXOR:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] ^ sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BLSHF:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] << sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BRSHF:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] >> sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BSRSHF:
			if (sfvm.SP < 2){
				vmerror("Stack too small.");
			}
			sfvm.stack_base[sfvm.SP - 1] = ((signed int)sfvm.stack_base[sfvm.SP - 1]) >> sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::PUSH:
			if (sfvm.SP >= sfvm.stack_size - 1){
				vmerror("Stack overflow.");
			}
			sfvm.stack_base[++sfvm.SP] = sfvm.operation_data;
			break;
		case CODE::PUSHP:
			if (sfvm.SP >= sfvm.stack_size - 1){
				vmerror("Stack overflow.");
			}
			if (sfvm.operation_data & 0x80000000 || sfvm.operation_data >= sfvm.data_size){
				vmerror("Data access error.");
			}
			if (lookup(sfvm.operation_data, watchedaddr, MAXWATCHEDADDR) && 0x05){
				debug();
			}
			sfvm.stack_base[++sfvm.SP] = sfvm.data_base[sfvm.operation_data];
			break;
		case CODE::PUSHV:
			if (sfvm.SP < 1){
				vmerror("Stack too small.");
			}
			if (sfvm.SP >= sfvm.stack_size - 1){
				vmerror("Stack overflow.");
			}
			if (sfvm.stack_base[sfvm.SP - 1] & 0x80000000 || sfvm.stack_base[sfvm.SP - 1] >= sfvm.data_size){
				vmerror("Data access error.");
			}
			if (lookup(sfvm.stack_base[sfvm.SP], watchedaddr, MAXWATCHEDADDR) && 0x05){
				debug();
			}
			sfvm.stack_base[++sfvm.SP] = sfvm.data_base[sfvm.stack_base[sfvm.SP - 1]];
			break;
		case CODE::PUSHT:
			if (sfvm.SP + 1 >= sfvm.stack_size){
				vmerror("Stack overflow.");
			}
			if (sfvm.SP - sfvm.operation_data < 1){
				vmerror("Illegal offset.");
			}
			sfvm.stack_base[sfvm.SP + 1] = sfvm.stack_base[sfvm.SP - sfvm.operation_data];
			sfvm.SP++;
			break;
		case CODE::COPYN:
			if (sfvm.SP + sfvm.operation_data >= sfvm.stack_size){
				vmerror("Stack overflow.");
			}
			for (unsigned int i = 1; i <= sfvm.operation_data; i++)
				sfvm.stack_base[sfvm.SP + i] = sfvm.stack_base[sfvm.SP - sfvm.operation_data + i];
			sfvm.SP += sfvm.operation_data;
			break;
		case CODE::COPYRN:
			if (sfvm.SP + sfvm.operation_data >= sfvm.stack_size){
				vmerror("Stack overflow.");
			}
			for (unsigned int i = 1; i <= sfvm.operation_data; i++)
				sfvm.stack_base[sfvm.SP + i] = sfvm.stack_base[sfvm.SP - i + 1];
			sfvm.SP += sfvm.operation_data;
			break;
		case CODE::POP:
			if (sfvm.SP < sfvm.operation_data){
				vmerror("Illegal data.");
			}
			sfvm.SP -= sfvm.operation_data;
			break;
		case CODE::STORE:
			if (sfvm.operation_data >= sfvm.data_size){
				vmerror("Data overflow.");
			}
			if (lookup(sfvm.operation_data, watchedaddr, MAXWATCHEDADDR) && 0x06){
				debug();
			}
			sfvm.data_base[sfvm.operation_data] = sfvm.stack_base[sfvm.SP];
			break;
		case CODE::STOREV:
			if (sfvm.SP <= 1){
				vmerror("Stack too small.");
			}
			if (sfvm.SP >= sfvm.data_size){
				vmerror("Data overflow.");
			}
			if (lookup(sfvm.stack_base[sfvm.SP - 1], watchedaddr, MAXWATCHEDADDR) && 0x06){
				debug();
			}
			sfvm.data_base[sfvm.stack_base[sfvm.SP - 1]] = sfvm.stack_base[sfvm.SP];
			break;
		case CODE::HALT:
			std::cout << "\n\tVM stepped into halt...\n";
			while (true);
			break;
		case CODE::JMP:
			if (sfvm.operation_data & 0x80000000)
				sfvm.PC -= sfvm.operation_data & 0x7fffffff;
			else
				sfvm.PC += sfvm.operation_data & 0x7fffffff;
			if (sfvm.PC >= sfvm.program_szie){
				vmerror("Illegal offset.");
			}
			break;
		case CODE::JTRUE:
			if (sfvm.stack_base[sfvm.SP]){
				if (sfvm.operation_data & 0x80000000)
					sfvm.PC -= sfvm.operation_data & 0x7fffffff;
				else
					sfvm.PC += sfvm.operation_data & 0x7fffffff;
			}
			if (sfvm.PC >= sfvm.program_szie){
				vmerror("Illegal offset.");
			}
			break;
		case CODE::JFALSE:
			if (!sfvm.stack_base[sfvm.SP]){
				if (sfvm.operation_data & 0x80000000)
					sfvm.PC -= sfvm.operation_data & 0x7fffffff;
				else
					sfvm.PC += sfvm.operation_data & 0x7fffffff;
			}
			if (sfvm.PC >= sfvm.program_szie){
				vmerror("Illegal offset.");
			}
			break;
		case CODE::JTRUEPOP:
			if (sfvm.stack_base[sfvm.SP]){
				if (sfvm.operation_data & 0x80000000)
					sfvm.PC -= sfvm.operation_data & 0x7fffffff;
				else
					sfvm.PC += sfvm.operation_data & 0x7fffffff;
				sfvm.SP--;
			}
			if (sfvm.PC >= sfvm.program_szie){
				vmerror("Illegal offset.");
			}
			break;
		case CODE::JFALSEPOP:
			if (!sfvm.stack_base[sfvm.SP]){
				if (sfvm.operation_data & 0x80000000)
					sfvm.PC -= sfvm.operation_data & 0x7fffffff;
				else
					sfvm.PC += sfvm.operation_data & 0x7fffffff;
				sfvm.SP--;
			}
			if (sfvm.PC >= sfvm.program_szie){
				vmerror("Illegal offset.");
			}
			break;
		case CODE::CALL:
			sfvm.stack_base[++sfvm.SP] = sfvm.PC;
			sfvm.PC = sfvm.operation_data;
			sfvm.temp_data_pointer++;
			if (sfvm.temp_data_pointer >= sfvm.temp_data_size / TD_SIZE){
				vmerror("Max temp data region overflow.");
			}
			break;
		case CODE::RET:
			sfvm.PC = sfvm.stack_base[sfvm.SP - sfvm.operation_data];
			for (int i = sfvm.operation_data; i > 0; i--)
				sfvm.stack_base[sfvm.SP - i] = sfvm.stack_base[sfvm.SP - i + 1];
			if (sfvm.SP <= 1 || sfvm.temp_data_pointer < 1){
				vmerror("Error in ret.");
			}
			sfvm.SP--;
			sfvm.temp_data_pointer--;
			break;
		case CODE::LPUSHP:
			if (sfvm.operation_data >= TD_SIZE){
				vmerror("Temp data overflow.");
			}
			sfvm.stack_base[++sfvm.SP] = sfvm.temp_data_base[sfvm.temp_data_pointer * TD_SIZE + sfvm.operation_data];
			break;
		case CODE::LPUSHV:
			if (sfvm.stack_base[sfvm.SP] >= TD_SIZE){
				vmerror("Temp data overflow.");
			}
			sfvm.stack_base[sfvm.SP + 1] = sfvm.temp_data_base[sfvm.temp_data_pointer * TD_SIZE + sfvm.stack_base[sfvm.SP]];
			sfvm.SP++;
			break;
		case CODE::LSTORE:
			if (sfvm.operation_data >= TD_SIZE){
				vmerror("Temp data overflow.");
			}
			sfvm.temp_data_base[sfvm.temp_data_pointer * TD_SIZE + sfvm.operation_data] = sfvm.stack_base[sfvm.SP];
			break;
		case CODE::LSTOREV:
			if (sfvm.stack_base[sfvm.SP - 1] >= TD_SIZE){
				vmerror("Temp data overflow.");
			}
			sfvm.temp_data_base[sfvm.temp_data_pointer * TD_SIZE + sfvm.stack_base[sfvm.SP - 1]] = sfvm.stack_base[sfvm.SP];
			break;
		case CODE::IN:
			if (sfvm.SP >= sfvm.stack_size){
				vmerror("Stack overflow.");
			}
			std::wcin >> get_c;
			sfvm.stack_base[++sfvm.SP] = get_c;
			break;
		case CODE::INNUM:
			if (sfvm.SP >= sfvm.stack_size){
				vmerror("Stack overflow.");
			}
			std::cin >> get_i;
			sfvm.stack_base[++sfvm.SP] = get_i;
			break;
		case CODE::OUT:
			std::wcout << (wchar_t)sfvm.stack_base[sfvm.SP];
			break;
		case CODE::OUTNUM:
			std::cout << sfvm.stack_base[sfvm.SP];
			break;
		}
	}
}