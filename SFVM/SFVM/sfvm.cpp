#include"sfvm.h"
#include<cstdlib>
#include<iostream>
struct SFVM sfvm;
int sfvm_init(unsigned int stack_size, unsigned int temp_data_size, unsigned data_size, unsigned int program_size){
	sfvm.PC = 0;
	sfvm.main_data_base = (unsigned int*)malloc((stack_size + temp_data_size + data_size + (program_size + 3) / 4) * sizeof(unsigned int));

	sfvm.program_base = (unsigned char*)sfvm.main_data_base;
	sfvm.program_szie = program_size;

	sfvm.data_base = sfvm.main_data_base + program_size;
	sfvm.data_size = data_size;

	sfvm.temp_data_base = sfvm.data_base + sfvm.data_size;
	sfvm.temp_data_size = temp_data_size;
	sfvm.temp_data_pointer = 0;

	sfvm.stack_base = sfvm.temp_data_base + sfvm.temp_data_size;
	sfvm.stack_size = stack_size;
	sfvm.SP = 0;

	std::locale loc("chs");
	std::wcout.imbue(loc);
	std::wcin.imbue(loc);

	return 0;
}

int sfvm_load(char *filename){					//加载二进制可执行文件
	FILE *file;
	unsigned int program_size = 0, data_size = 0;
	char buf[64];
	strcpy_s(buf, filename);
	fopen_s(&file, buf, "r");
	if (!file){
		std::cout << "File not exist.\n";
	}
	else{
		fread(buf, 1, 4, file);
		for (int i = 0; i < 4; i++){
			data_size = data_size << 8;
			data_size |= buf[3 - i];
		}
		fread(buf, 1, 4, file);
		for (int i = 0; i < 4; i++){
			program_size = program_size << 8;
			program_size |= (unsigned char)buf[3 - i];
		}

		fread(buf, 1, 32 - 8, file);
		
		sfvm_init(10240, 102400, data_size, program_size);
		fread(sfvm.data_base, 4, sfvm.data_size, file);
		fread(sfvm.main_data_base, 1, program_size, file);
	}
	fclose(file);
	return 0;
}

int sfvm_run(){
	int code = 0;
	int get_i;
	wchar_t get_c;
	sfvm.PC = 0;
	while (true){							//模拟硬件执行：
		/*
		读进PC指向的指令并译码（这里做的只有判断是否有操作数）
		*/
		code = sfvm.program_base[sfvm.PC++];
		if (code & 0x80){
			sfvm.operation_data = 0x00000000;
			for (int i = 0; i < 4; i++){
				sfvm.operation_data |= (sfvm.program_base[sfvm.PC++] << (i * 8));
			}
		}
		switch (code & 0x7f){				//switch里面是些细节实现（由于没能处理好64位和32位系统对于int类型的2的补码，这里使用的是1的补码）
		case CODE::NOP:						//每个case对应的是每条指令的实现
			break;
		case CODE::NEG:
			sfvm.stack_base[sfvm.SP] = -((signed int)sfvm.stack_base[sfvm.SP]);
			break;
		case CODE::ADD:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] + sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::SUB:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] - sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::MUL:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] * sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::DIV:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] / sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::MOD:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] % sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::NOT:
			sfvm.stack_base[sfvm.SP] = !sfvm.stack_base[sfvm.SP];
			break;
		case CODE::LESS:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] < sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::GRT:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] > sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::LESSE:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] <= sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::GRTE:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] >= sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::EQU:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] == sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::NEQU:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] != sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::AND:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] && sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::OR:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] || sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BNOT:
			sfvm.stack_base[sfvm.SP] = ~sfvm.stack_base[sfvm.SP];
			break;
		case CODE::BAND:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] & sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BOR:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] | sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BXOR:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] ^ sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BLSHF:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] << sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BRSHF:
			sfvm.stack_base[sfvm.SP - 1] = sfvm.stack_base[sfvm.SP - 1] >> sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::BSRSHF:
			sfvm.stack_base[sfvm.SP - 1] = ((signed int)sfvm.stack_base[sfvm.SP - 1]) >> sfvm.stack_base[sfvm.SP];
			sfvm.SP--;
			break;
		case CODE::PUSH:
			sfvm.stack_base[++sfvm.SP] = sfvm.operation_data;
			break;
		case CODE::PUSHP:
			sfvm.stack_base[++sfvm.SP] = sfvm.data_base[sfvm.operation_data];
			break;
		case CODE::PUSHV:
			sfvm.stack_base[++sfvm.SP] = sfvm.data_base[sfvm.stack_base[sfvm.SP - 1]];
			break;
		case CODE::PUSHT:
			sfvm.stack_base[sfvm.SP + 1] = sfvm.stack_base[sfvm.SP - sfvm.operation_data];
			sfvm.SP++;
			break;
		case CODE::COPYN:
			for (unsigned int i = 1; i <= sfvm.operation_data; i++)
				sfvm.stack_base[sfvm.SP + i] = sfvm.stack_base[sfvm.SP - sfvm.operation_data + i];
			sfvm.SP += sfvm.operation_data;
			break;
		case CODE::COPYRN:
			for (unsigned int i = 1; i <= sfvm.operation_data; i++)
				sfvm.stack_base[sfvm.SP + i] = sfvm.stack_base[sfvm.SP - i + 1];
			sfvm.SP += sfvm.operation_data;
			break;
		case CODE::POP:
			sfvm.SP -= sfvm.operation_data;
			break;
		case CODE::STORE:
			sfvm.data_base[sfvm.operation_data] = sfvm.stack_base[sfvm.SP];
			break;
		case CODE::STOREV:
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
			break;
		case CODE::JTRUE:
			if (sfvm.stack_base[sfvm.SP]){
				if (sfvm.operation_data & 0x80000000)
					sfvm.PC -= sfvm.operation_data & 0x7fffffff;
				else
					sfvm.PC += sfvm.operation_data & 0x7fffffff;
			}
			break;
		case CODE::JFALSE:
			if (!sfvm.stack_base[sfvm.SP]){
				if (sfvm.operation_data & 0x80000000)
					sfvm.PC -= sfvm.operation_data & 0x7fffffff;
				else
					sfvm.PC += sfvm.operation_data & 0x7fffffff;
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
			break;
		case CODE::JFALSEPOP:
			if (!sfvm.stack_base[sfvm.SP]){
				if (sfvm.operation_data & 0x80000000)
					sfvm.PC -= sfvm.operation_data & 0x7fffffff;
				else
					sfvm.PC += sfvm.operation_data & 0x7fffffff;
				sfvm.SP--;
			}
			break;
		case CODE::CALL:
			sfvm.stack_base[++sfvm.SP] = sfvm.PC;
			sfvm.PC = sfvm.operation_data;
			sfvm.temp_data_pointer++;
			break;
		case CODE::RET:
			sfvm.PC = sfvm.stack_base[sfvm.SP - sfvm.operation_data];
			for (int i = sfvm.operation_data; i > 0; i--)
				sfvm.stack_base[sfvm.SP - i] = sfvm.stack_base[sfvm.SP - i + 1];
			sfvm.SP--;
			sfvm.temp_data_pointer--;
			break;
		case CODE::LPUSHP:
			sfvm.stack_base[++sfvm.SP] = sfvm.temp_data_base[sfvm.temp_data_pointer * TD_SIZE + sfvm.operation_data];
			break;
		case CODE::LPUSHV:
			sfvm.stack_base[sfvm.SP + 1] = sfvm.temp_data_base[sfvm.temp_data_pointer * TD_SIZE + sfvm.stack_base[sfvm.SP]];
			sfvm.SP++;
			break;
		case CODE::LSTORE:
			sfvm.temp_data_base[sfvm.temp_data_pointer * TD_SIZE + sfvm.operation_data] = sfvm.stack_base[sfvm.SP];
			break;
		case CODE::LSTOREV:
			sfvm.temp_data_base[sfvm.temp_data_pointer * TD_SIZE + sfvm.stack_base[sfvm.SP - 1]] = sfvm.stack_base[sfvm.SP];
			break;
		case CODE::IN:
			std::wcin >> get_c;
			sfvm.stack_base[++sfvm.SP] = get_c;
			break;
		case CODE::INNUM:
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

int main(int argc, char* argv[]){
	bool flag_debug = false;
	flag_debug = true;
	if (argc <= 1){
		printf("No input file.\n");
		return 1;
	}
	if (!sfvm_load(argv[1])){
		if (argc > 2){
			if (argc > 3)
				sfvm_debug(atoi(argv[3]));
			else sfvm_debug(-1);
		}
		else{
			sfvm_run();
		}
	}
	return 0;
}

int smain(){
	sfvm_load("out.bin");
	sfvm_debug(-1);
	//sfvm_run();
	return 0;
}