#ifndef COMQUEUE_H
#define COMQUEUE_H

#include <stdio.h>
#include <inttypes.h>
#include <stddef.h>
#include <malloc.h>

#include "inter.h"

struct SNode{
	union UCom * com;
	struct SNode * next;
};

struct SQueue{
	size_t count;
	struct SNode * first;
	struct SNode * last;
};

//Node methods
struct SNode * node_create();
void node_append(struct SNode ** node, union UCom * com);

//Queue methods
struct SQueue * queue_create();
void queue_push(struct SQueue * q, union UCom * com);
union UCom * queue_pop(struct SQueue * q);
void queue_print(struct SQueue * q);
void queue_free(struct SQueue * q);
union UCom * queue_toarray(struct SQueue * q);

const union UCom * queue_getprogram();

#endif
