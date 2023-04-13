#ifndef STACK_H
#define STACK_H

#include <stddef.h>
#include <stdint.h>

typedef struct stack stack_t;

struct stack
{
    uint32_t *ebp;
    uint32_t *esp;

    uint32_t *_stackMem;

    uint32_t (*pop)(stack_t*);
    void (*push)(stack_t*, uint32_t);
    void (*createFrame)(stack_t*);
};

stack_t * createStack(size_t);

void myCall(stack_t *stack, void (*func)());

#endif
