#include "stack.h"
#include <stdlib.h>

uint32_t pop(stack_t *stack)
{
    uint32_t value = *(stack->esp);
    if (stack->esp < stack->ebp)
        stack->esp += 1;
    return value;
}

void push(stack_t *stack, uint32_t data)
{
    stack->esp -= 1;
    *(stack->esp) = data;
}

void createFrame(stack_t *stack)
{
    unsigned numStackPos = sizeof(stack->ebp) / sizeof(uint32_t);
    uint32_t* ebpDir = (uint32_t *) &(stack->ebp);

    uint32_t * tempEsp = stack->esp;
    for (int i=0; i<numStackPos; ++i)
        stack->push(stack, ebpDir[i]);

    stack->ebp = tempEsp;
}

stack_t * createStack(size_t size)
{
    stack_t *stack = malloc(sizeof(stack_t));

    size_t totalSize = size * sizeof(uint32_t);

    stack->_stackMem = malloc(totalSize);
    stack->ebp = stack->_stackMem + size - 1;  // La base del stack es la dirección más grande.
    stack->esp = stack->ebp;

    stack->pop = pop;
    stack->push = push;
    stack->createFrame = createFrame;

    return stack;
}

void myCall(stack_t *stack, void (*func)())
{
    func();

    unsigned numStackPos = sizeof(stack->ebp) / sizeof(uint32_t);
    uint32_t* ebpDir = (uint32_t *) &(stack->ebp);

    for (int i=(numStackPos-1); i>=0; --i)
        ebpDir[i] = stack->pop(stack);
}
