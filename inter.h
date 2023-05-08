#ifndef INTER_H
#define INTER_H

#include <stdio.h>
#include <malloc.h>
#include <inttypes.h>
#include <stdbool.h>

// operation codes
enum EOpCode{
	EOC_LD, EOC_LDN, EOC_ST, EOC_STN,
	EOC_READ, EOC_PRINT,
	EOC_AND, EOC_ANDN, EOC_OR, EOC_ORN, EOC_XOR, EOC_XORN, EOC_NOT,
	EOC_ADD, EOC_SUB, EOC_MUL, EOC_DIV,
	EOC_GT, EOC_GE, EOC_EQ, EOC_NE, EOC_LE, EOC_LT,
	EOC_JMP, EOC_JMPC, EOC_JMPCN,
	EOC_RET, EOC_RETC, EOC_RETCN
};

enum EArgType{
	EAT_NOARG,
	EAT_INT64,
	EAT_FLOAT
};

struct SDescr{
	char * mnemonic;
	enum EArgType argtype;
	bool affects_com;
};

// operator structures
struct SComNoArg{

	enum EOpCode opcode;
};

struct SComArg64{

	enum EOpCode opcode;
	int64_t arg;
};

// Command
union UCom{

	enum EOpCode opcode;
	//struct SComNoArg noArg;
	struct SComArg64 com_arg64;
};

extern const struct SDescr instructions[];

// Accumulator
struct SAccum{

	int64_t value;
};

struct SAccum accum_create();
void accum_set(struct SAccum * accum, int64_t value);

int64_t read_int64();

// State of program
struct SState{
	const union UCom * curcom;
	struct SAccum accum;
	bool is_run;
};

struct SState state_create(const union UCom * program);

// Unary operations
void set_unop(struct SAccum * accum, int64_t (f)(int64_t));
int64_t i64_not(int64_t a);

// Binary opperations
void set_binop(struct SState * state, int64_t (f)(int64_t, int64_t));
int64_t i64_and(int64_t a, int64_t b);
int64_t i64_andn(int64_t a, int64_t b);
int64_t i64_or(int64_t a, int64_t b);
int64_t i64_orn(int64_t a, int64_t b);
int64_t i64_xor(int64_t a, int64_t b);
int64_t i64_xorn(int64_t a, int64_t b);
int64_t i64_add(int64_t a, int64_t b);
int64_t i64_sub(int64_t a, int64_t b);
int64_t i64_mul(int64_t a, int64_t b);
int64_t i64_div(int64_t a, int64_t b);
int64_t i64_gt(int64_t a, int64_t b);
int64_t i64_ge(int64_t a, int64_t b);
int64_t i64_eq(int64_t a, int64_t b);
int64_t i64_ne(int64_t a, int64_t b);
int64_t i64_le(int64_t a, int64_t b);
int64_t i64_lt(int64_t a, int64_t b);

// Interpretator operations
void inter_ld(struct SState * state);
void inter_ldn(struct SState * state);
void inter_print(struct SState * state);
void inter_st(struct SState * state);
void inter_stn(struct SState * state);
void inter_read(struct SState * state);
void inter_and(struct SState * state);
void inter_andn(struct SState * state);
void inter_or(struct SState * state);
void inter_orn(struct SState * state);
void inter_xor(struct SState * state);
void inter_xorn(struct SState * state);
void inter_not(struct SState * state);
void inter_add(struct SState * state);
void inter_sub(struct SState * state);
void inter_mul(struct SState * state);
void inter_div(struct SState * state);
void inter_gt(struct SState * state);
void inter_ge(struct SState * state);
void inter_eq(struct SState * state);
void inter_ne(struct SState * state);
void inter_le(struct SState * state);
void inter_lt(struct SState * state);
void inter_jmp(struct SState * state);
void inter_jmpc(struct SState * state);
void inter_jmpcn(struct SState * state);
void inter_ret(struct SState * state);
void inter_retc(struct SState * state);
void inter_retcn(struct SState * state);

// Commands interpretator
void interpret(struct SState * state);
void interpret_program(const union UCom * program);

#endif
