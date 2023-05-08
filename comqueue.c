#include <stdio.h>
#include <inttypes.h>
#include <stddef.h>
#include <malloc.h>

#include "comqueue.h"

//Node methods
struct SNode * node_create(){
	struct SNode * node = malloc(sizeof(struct SNode));
	return node;
}

void node_append(struct SNode ** node, union UCom * com){
	if (!(*node)){
		*node = node_create();
		(*node)->com = com;
		return;
	}

	if ((*node)->next){
		printf("Node already has next node");
		return;
	}

	(*node)->next = node_create();
	(*node)->next->com = com;
}

//Queue methods
struct SQueue * queue_create(){
	struct SQueue * queue = malloc(sizeof(struct SQueue));
	queue->count = 0;
	return queue;
}

void queue_push(struct SQueue * q, union UCom * com){
	q->count++;
	node_append(&q->last, com);
	if (!q->first){
		q->first = q->last;
		return;
	}
	q->last = q->last->next;
}

union UCom * queue_pop(struct SQueue * q){
	if (!q->first){
		printf("No elements in queue\n");
		return NULL;
	}
	q->count--;
	union UCom * res = q->first->com;
	q->first = q->first->next;
	return res;
}

union UCom * queue_toarray(struct SQueue * q){
	size_t count = q->count;
	if (count == 0) return NULL;
	
	union UCom * program = malloc(count * sizeof(union UCom));

	for (size_t i = 0; i < count; i++)
		program[i] = *queue_pop(q);

	return program;
}

void com_print(union UCom * com){
	if (!com) return;
	printf("%s", instructions[com->opcode]);
	if (instructions[com->opcode].argtype == EAT_INT64){
		printf(" %"PRId64, com->com_arg64.arg);
	}
}

void queue_print(struct SQueue * q){
	if (q){
		struct SNode * curnode = q->first;
		while (curnode){
			com_print(curnode->com);
			printf(" -> ");
			curnode = curnode->next;
		}
	}
	printf("NULL\n");
}

void queue_free(struct SQueue * q){
	if (!q){
		printf("Queue is already empty\n");
		return;
	}
	struct SNode * node = q->first;
	while (node){
		struct SNode * newnode = node->next;
		free(node);
		node = newnode;
	}
	free(q);
}

void array_print(union UCom * arr, size_t size){
	for (size_t i = 0; i < size; i++){
		com_print(&arr[i]);
		printf("\n");
	}
}

const union UCom * queue_getprogram(){
	struct SQueue * queue = queue_create();
	queue_push(queue, &(union UCom){EOC_READ});
	queue_push(queue, &(union UCom){.com_arg64 = {EOC_ADD, .arg = 10}});
	queue_push(queue, &(union UCom){EOC_PRINT});
	queue_push(queue, &(union UCom){EOC_RET});
	queue_print(queue);
	printf("Queue count: %zu\n", queue->count);
	union UCom * program = queue_toarray(queue);
 	//array_print(program, queue->count);
	queue_free(queue);

	return program;
}

/*int main(){
	const union UCom * program = queue_getprogram();
}*/
