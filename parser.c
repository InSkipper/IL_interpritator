#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "parser.h"
#include "inter.h"
#include "comqueue.h"

const struct SUnop com_tounop[] = {
//	mnemonic	func()
	{"not", 	parser_not},
	{"ret", 	parser_ret},
	{"retc", 	parser_retc},
	{"retcn", 	parser_retcn},
	{"read", 	parser_read},
	{"print", 	parser_print}
};

const struct SBinop com_tobinop[] = {
//	mnemonic	func(int64_t arg)
	{"ld",		parser_ld},
	{"ldn",		parser_ldn},
	{"st",		parser_st},
	{"stn",		parser_stn},

	{"and",		parser_and},
	{"andn",	parser_andn},
	{"or",	 	parser_or},
	{"orn", 	parser_orn},
	{"xor", 	parser_xor},
	{"xorn", 	parser_xorn},

	{"add", 	parser_add},
	{"sub", 	parser_sub},
	{"mul", 	parser_mul},
	{"div", 	parser_div},

	{"gt", 		parser_gt},
	{"ge", 		parser_ge},
	{"eq", 		parser_eq},
	{"ne", 		parser_ne},
	{"le", 		parser_le},
	{"lt", 		parser_lt},

	{"jmp", 	parser_jmp},
	{"jmpc", 	parser_jmpc},
	{"jmpcn", 	parser_jmpcn}
};

// unary operations
union UCom * parser_setunop(enum EOpCode opcode){
	union UCom * res = malloc(sizeof(union UCom));
	*res = (union UCom){opcode};
	return res;
}

union UCom * parser_not(){
	return parser_setunop(EOC_NOT);
}

union UCom * parser_ret(){
	return parser_setunop(EOC_RET);
}

union UCom * parser_retc(){
	return parser_setunop(EOC_RETC);
}

union UCom * parser_retcn(){
	return parser_setunop(EOC_RETCN);
}

union UCom * parser_read(){
	return parser_setunop(EOC_READ);
}

union UCom * parser_print(){
	return parser_setunop(EOC_PRINT);
}

// binary operations
union UCom * parser_setbinop(enum EOpCode opcode, int64_t arg){
	union UCom * res = malloc(sizeof(union UCom));
	*res = (union UCom){.com_arg64 = {opcode, .arg = arg}};
	return res;
}

union UCom * parser_ld(int64_t arg){
	return parser_setbinop(EOC_LD, arg);
}

union UCom * parser_ldn(int64_t arg){
	return parser_setbinop(EOC_LDN, arg);
}

union UCom * parser_st(int64_t arg){
	return parser_setbinop(EOC_ST, arg);
}

union UCom * parser_stn(int64_t arg){
	return parser_setbinop(EOC_STN, arg);
}

union UCom * parser_and(int64_t arg){
	return parser_setbinop(EOC_AND, arg);
}

union UCom * parser_andn(int64_t arg){
	return parser_setbinop(EOC_ANDN, arg);
}

union UCom * parser_or(int64_t arg){
	return parser_setbinop(EOC_OR, arg);
}

union UCom * parser_orn(int64_t arg){
	return parser_setbinop(EOC_ORN, arg);
}

union UCom * parser_xor(int64_t arg){
	return parser_setbinop(EOC_XOR, arg);
}

union UCom * parser_xorn(int64_t arg){
	return parser_setbinop(EOC_XORN, arg);
}

union UCom * parser_add(int64_t arg){
	return parser_setbinop(EOC_ADD, arg);
}

union UCom * parser_sub(int64_t arg){
	return parser_setbinop(EOC_SUB, arg);
}

union UCom * parser_mul(int64_t arg){
	return parser_setbinop(EOC_MUL, arg);
}

union UCom * parser_div(int64_t arg){
	return parser_setbinop(EOC_DIV, arg);
}

union UCom * parser_gt(int64_t arg){
	return parser_setbinop(EOC_GT, arg);
}

union UCom * parser_ge(int64_t arg){
	return parser_setbinop(EOC_GE, arg);
}

union UCom * parser_eq(int64_t arg){
	return parser_setbinop(EOC_EQ, arg);
}

union UCom * parser_ne(int64_t arg){
	return parser_setbinop(EOC_NE, arg);
}

union UCom * parser_le(int64_t arg){
	return parser_setbinop(EOC_LE, arg);
}

union UCom * parser_lt(int64_t arg){
	return parser_setbinop(EOC_LT, arg);
}

union UCom * parser_jmp(int64_t arg){
	return parser_setbinop(EOC_JMP, arg);
}

union UCom * parser_jmpc(int64_t arg){
	return parser_setbinop(EOC_JMPC, arg);
}

union UCom * parser_jmpcn(int64_t arg){
	return parser_setbinop(EOC_JMPCN, arg);
}

union UCom * parser_getunop(char * name){
	size_t size = sizeof(com_tounop)/sizeof(struct SUnop);
	for (size_t i = 0; i < size; i++){
		if (strcmp(name, com_tounop[i].mnemonic) == 0){
			return com_tounop[i].func();
		}
	}
	return NULL;
}

union UCom * parser_getbinop(char * name, int64_t arg){
	size_t size = sizeof(com_tobinop)/sizeof(struct SBinop);
	for (size_t i = 0; i < size; i++){
		if (strcmp(name, com_tobinop[i].mnemonic) == 0){
			return com_tobinop[i].func(arg);
		}
	}
	return NULL;
}

struct SParser * parser_create(struct SQueue * comqueue){
	struct SParser * res = malloc(sizeof(struct SParser));
	*res = (struct SParser){EEC_NOERR, comqueue};
	return res;
}

void parser_free(struct SParser * parser){
	queue_free(parser->comqueue);
	free(parser);
}

void parser_handleop(struct SParser * parser, char * com, union UCom * op){
	if (!op){
		printf("ERROR: Unable to find command %s\n", com);
		parser->errcode = EEC_NOCOM;
		return;
	}
	if (parser->errcode == EEC_NOERR)
		queue_push(parser->comqueue, op);
}

/*const char * errcode_totext[] = {
	[EEC_NOCOM] = "No such command",
	[EEC_NOARG] = "Too few arguments",
	[EEC_NORET] = "Missing return statement in program",
	[EEC_TOOMANYARG] = "Too many arguments"
};

void error_print(size_t line, enum EErrCode errcode){
	printf("ERROR: %s\n", line, errcode_totext[errcode]);
}*/

void parse(FILE * file, struct SParser * parser){
	char com[50];
	int64_t param = 0;
	int res = 0;
	while (res != EOF){
		res = fscanf(file, "%49s %"SCNd64, com, &param);
		if (res == 1){
			printf("Command is %s\n", com);
			parser_handleop(parser, com, parser_getunop(com));
		}
		else if (res == 2){
			printf("Command is %s. Param is %"PRId64"\n", com, param);
			parser_handleop(parser, com, parser_getbinop(com, param));
		}
	}
	fclose(file);
}

void file_print(FILE * file){
	char str[256];
	char * estr;
	while (1){
		estr = fgets(str, 255, file);
		if (!estr) break;
		printf("%s", str);
	}
}

/*int main(){
	struct SQueue * comqueue = queue_create();
	struct SParser * parser = parser_create(comqueue);
	FILE * file = fopen("prog.txt", "r");
//	file_print(file);
	parse(file, parser);
//	queue_push(comqueue, &(union UCom){EOC_PRINT});
	printf("Test\n");
	queue_print(comqueue);

	return 0;
}
*/
