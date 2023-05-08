#include <stdio.h>
#include <malloc.h>
#include <inttypes.h>
#include <stdbool.h>

#include "inter.h"
#include "comqueue.h"
#include "parser.h"

const struct SDescr instructions[] = {
//	op_code			mnemonic	argtype		affects_com
	[EOC_LD] 	= {	"ld", 		EAT_INT64, 	false	},
	[EOC_LDN] 	= {	"ldn", 		EAT_INT64, 	false	},
	[EOC_ST] 	= {	"st", 		EAT_INT64, 	false	},
	[EOC_STN] 	= {	"stn", 		EAT_INT64, 	false	},
	[EOC_READ] 	= {	"ld in",	EAT_NOARG, 	false	},
	[EOC_PRINT] = {	"st out",	EAT_NOARG, 	false	},

	[EOC_AND] 	= {	"and", 		EAT_INT64, 	false	},
	[EOC_ANDN] 	= {	"andn",		EAT_INT64, 	false	},
	[EOC_OR] 	= {	"or", 		EAT_INT64, 	false	},
	[EOC_ORN] 	= {	"orn", 		EAT_INT64, 	false	},
	[EOC_XOR] 	= {	"xor", 		EAT_INT64, 	false	},
	[EOC_XORN] 	= {	"xorn",		EAT_INT64, 	false	},
	[EOC_NOT] 	= {	"not", 		EAT_NOARG, 	false	},

	[EOC_ADD] 	= {	"add", 		EAT_INT64, 	false	},
	[EOC_SUB] 	= {	"sub", 		EAT_INT64, 	false	},
	[EOC_MUL] 	= {	"mul", 		EAT_INT64, 	false	},
	[EOC_DIV] 	= {	"div", 		EAT_INT64, 	false	},

	[EOC_GT] 	= {	"gt", 		EAT_INT64, 	false	},
	[EOC_GE] 	= {	"ge", 		EAT_INT64, 	false	},
	[EOC_EQ] 	= {	"eq", 		EAT_INT64, 	false	},
	[EOC_NE] 	= {	"ne", 		EAT_INT64, 	false	},
	[EOC_LE] 	= {	"le", 		EAT_INT64, 	false	},
	[EOC_LT] 	= {	"lt", 		EAT_INT64, 	false	},

	[EOC_JMP] 	= {	"jmp", 		EAT_INT64, 	true	},
	[EOC_JMPC] 	= {	"jmpc",		EAT_INT64, 	true	},
	[EOC_JMPCN] = {	"jmpcn",	EAT_INT64, 	true	},

	[EOC_RET] 	= {	"ret", 		EAT_NOARG, 	false	},
	[EOC_RETC] 	= {	"retc",		EAT_NOARG, 	false	},
	[EOC_RETCN] = {	"retcn",	EAT_NOARG, 	false	}
};

// Accumulator
struct SAccum accum_create(){
	return (struct SAccum){.value = 0};
}

void accum_set(struct SAccum * accum, int64_t value){
	if (!accum) return;
	accum->value = value;
}

int64_t read_int64(){
	int64_t res = 0;
	scanf("%"SCNd64, &res);

	return res;
}

// State
struct SState state_create(const union UCom * program){
	return (struct SState){
		.curcom = program,
		.accum = accum_create(),
		.is_run = true
	};
}

// Unary operations
void set_unop(struct SAccum * accum, int64_t (f)(int64_t)){
	accum_set(accum, f(accum->value));
}

int64_t i64_not(int64_t a){
	return ~a;
}

// Binary opperations
void set_binop(struct SState * state, int64_t (f)(int64_t, int64_t)){
	accum_set(&state->accum, f(state->accum.value, state->curcom->com_arg64.arg));
}

int64_t i64_and(int64_t a, int64_t b){
	return a & b;
}

int64_t i64_andn(int64_t a, int64_t b){
	return ~(a & b);
}

int64_t i64_or(int64_t a, int64_t b){
	return a | b;
}

int64_t i64_orn(int64_t a, int64_t b){
	return ~(a | b);
}

int64_t i64_xor(int64_t a, int64_t b){
	return a ^ b;
}

int64_t i64_xorn(int64_t a, int64_t b){
	return ~(a ^ b);
}


int64_t i64_add(int64_t a, int64_t b){
	return a + b;
}

int64_t i64_sub(int64_t a, int64_t b){
	return a - b;
}

int64_t i64_mul(int64_t a, int64_t b){
	return a * b;
}

int64_t i64_div(int64_t a, int64_t b){
	return a / b;
}


int64_t i64_gt(int64_t a, int64_t b){
	return a > b ? 1 : 0;
}

int64_t i64_ge(int64_t a, int64_t b){
	return a >= b ? 1 : 0;
}

int64_t i64_eq(int64_t a, int64_t b){
	return a == b ? 1 : 0;
}

int64_t i64_ne(int64_t a, int64_t b){
	return a != b ? 1 : 0;
}

int64_t i64_le(int64_t a, int64_t b){
	return a <= b ? 1 : 0;
}

int64_t i64_lt(int64_t a, int64_t b){
	return a < b ? 1 : 0;
}

// Interpretator operations
void inter_ld(struct SState * state){
	accum_set(&state->accum, state->curcom->com_arg64.arg);
}

void inter_ldn(struct SState * state){
	accum_set(&state->accum, -state->curcom->com_arg64.arg);
}

void inter_print(struct SState * state){
	printf("%"PRId64"\n", state->accum.value);
}

void inter_st(struct SState * state){
	inter_print(state);
}

void inter_stn(struct SState * state){
	printf("-");
	inter_print(state);
}


void inter_read(struct SState * state){
	accum_set(&state->accum, read_int64());
}


void inter_and(struct SState * state){
	set_binop(state, i64_and);
}

void inter_andn(struct SState * state){
	set_binop(state, i64_andn);
}

void inter_or(struct SState * state){
	set_binop(state, i64_or);
}

void inter_orn(struct SState * state){
	set_binop(state, i64_orn);
}

void inter_xor(struct SState * state){
	set_binop(state, i64_xor);
}

void inter_xorn(struct SState * state){
	set_binop(state, i64_xorn);
}

void inter_not(struct SState * state){
	set_unop(&state->accum, i64_not);
}


void inter_add(struct SState * state){
	set_binop(state, i64_add);
}

void inter_sub(struct SState * state){
	set_binop(state, i64_sub);
}

void inter_mul(struct SState * state){
	set_binop(state, i64_mul);
}

void inter_div(struct SState * state){
	set_binop(state, i64_div);
}


void inter_gt(struct SState * state){
	set_binop(state, i64_gt);
}

void inter_ge(struct SState * state){
	set_binop(state, i64_ge);
}

void inter_eq(struct SState * state){
	set_binop(state, i64_eq);
}

void inter_ne(struct SState * state){
	set_binop(state, i64_ne);
}

void inter_le(struct SState * state){
	set_binop(state, i64_le);
}

void inter_lt(struct SState * state){
	set_binop(state, i64_lt);
}


void inter_jmp(struct SState * state){
	state->curcom += state->curcom->com_arg64.arg;
}

void inter_jmpc(struct SState * state){
	if (state->accum.value == 1){
		state->curcom += state->curcom->com_arg64.arg;
	}
	else{
		state->curcom += 1;
	}
}

void inter_jmpcn(struct SState * state){
	if (state->accum.value == 0){
		state->curcom += state->curcom->com_arg64.arg;
	}
	else{
		state->curcom += 1;
	}
}


void inter_ret(struct SState * state){
	state->is_run = false;
}

void inter_retc(struct SState * state){
	if (state->accum.value == 1)
		state->is_run = false;
}

void inter_retcn(struct SState * state){
	if (state->accum.value == 0)
		state->is_run = false;
}

// Operation code to funtion
void (*interprets[])(struct SState * state) = {
	[EOC_LD] = inter_ld,
	[EOC_LDN] = inter_ldn,
	[EOC_ST] = inter_st,
	[EOC_STN] = inter_stn,
	[EOC_READ] = inter_read,
	[EOC_PRINT] = inter_print,

	[EOC_AND] = inter_and,
	[EOC_ANDN] = inter_andn,
	[EOC_OR] = inter_or,
	[EOC_ORN] = inter_orn,
	[EOC_XOR] = inter_xor,
	[EOC_XORN] = inter_xorn,
	[EOC_NOT] = inter_not,

	[EOC_ADD] = inter_add,
	[EOC_SUB] = inter_sub,
	[EOC_MUL] = inter_mul,
	[EOC_DIV] = inter_div,

	[EOC_GT] = inter_gt,
	[EOC_GE] = inter_ge,
	[EOC_EQ] = inter_eq,
	[EOC_NE] = inter_ne,
	[EOC_LE] = inter_le,
	[EOC_LT] = inter_lt,

	[EOC_JMP] = inter_jmp,
	[EOC_JMPC] = inter_jmpc,
	[EOC_JMPCN] = inter_jmpcn,

	[EOC_RET] = inter_ret,
	[EOC_RETC] = inter_retc,
	[EOC_RETCN] = inter_retcn
};

// Commands interpretator
void interpret(struct SState * state){
	while (state->curcom){
		enum EOpCode opcode = state->curcom->opcode;
		if (interprets[opcode]){
			interprets[opcode](state);
		}
		else {
			char * mnemonic = instructions[opcode].mnemonic;
			printf("No such command: %s\n", mnemonic);
			return;
		}

		if (!instructions[opcode].affects_com)
			state->curcom++;

		if (!state->is_run){
			return;
		}
	}
}

void interpret_program(const union UCom * program){

	struct SState state = state_create(program);
	interpret(&state);
}

int main(){
//	const union UCom * program = queue_getprogram();

	struct SQueue * comqueue = queue_create();
	struct SParser * parser = parser_create(comqueue);
	FILE * file = fopen("prog.txt", "r");
	if (!file) {
		printf("Unable to open file\n");
		return 0;
	}

	parse(file, parser);

	if (parser->errcode != EEC_NOERR){
		printf("Parsing falt\n");
		return 0;
	}

	queue_print(comqueue);
	const union UCom * program = queue_toarray(comqueue);
	interpret_program(program);
}
