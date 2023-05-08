#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "comqueue.h"

// unary operations
union UCom * parser_setunop(enum EOpCode opcode);
union UCom * parser_not();
union UCom * parser_ret();
union UCom * parser_retc();
union UCom * parser_retcn();
union UCom * parser_read();
union UCom * parser_print();

// binary operations
union UCom * parser_setbinop(enum EOpCode opcode, int64_t arg);
union UCom * parser_ld(int64_t arg);
union UCom * parser_ldn(int64_t arg);
union UCom * parser_st(int64_t arg);
union UCom * parser_stn(int64_t arg);
union UCom * parser_and(int64_t arg);
union UCom * parser_andn(int64_t arg);
union UCom * parser_or(int64_t arg);
union UCom * parser_orn(int64_t arg);
union UCom * parser_xor(int64_t arg);
union UCom * parser_xorn(int64_t arg);
union UCom * parser_add(int64_t arg);
union UCom * parser_sub(int64_t arg);
union UCom * parser_mul(int64_t arg);
union UCom * parser_div(int64_t arg);
union UCom * parser_gt(int64_t arg);
union UCom * parser_ge(int64_t arg);
union UCom * parser_eq(int64_t arg);
union UCom * parser_ne(int64_t arg);
union UCom * parser_le(int64_t arg);
union UCom * parser_lt(int64_t arg);
union UCom * parser_jmp(int64_t arg);
union UCom * parser_jmpc(int64_t arg);
union UCom * parser_jmpcn(int64_t arg);

struct SUnop{
	char * mnemonic;
	union UCom * (*func)();
};

struct SBinop{
	char * mnemonic;
	union UCom * (*func)(int64_t arg);
};

union UCom * parser_getunop(char * name);
union UCom * parser_getbinop(char * name, int64_t arg);

enum EErrCode{
	EEC_NOERR,
	EEC_NORET,
	EEC_NOCOM,
	EEC_NOARG,
	EEC_TOOMANYARG
};

struct SParser{
	enum EErrCode errcode;
	struct SQueue * comqueue;
};

struct SParser * parser_create(struct SQueue * comqueue);
void parser_free(struct SParser * parser);
void parser_handleop(struct SParser * parser, char * com, union UCom * op);
//void error_print(size_t line, enum EErrCode errcode);
void parse(FILE * file, struct SParser * parser);
void file_print(FILE * file);

#endif
