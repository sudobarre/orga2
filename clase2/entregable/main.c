#include <stdio.h>
#include <stdlib.h>

#include "stack.h"
#include "student.h"

stack_t *stack;

int main()
{
    stack = createStack(100);

    student_t stud1 = {
        .name = "Steve Balmer",
        .dni = 12345678,
        .califications = {3,2,1},
        .concept = -2,
    };

    studentp_t stud2 = {
        .name = "Linus Torvalds",
        .dni = 23456789,
        .califications = {9,7,8},
        .concept = 1,
    };


    uint32_t *studp;

    // Hint: ver 'createFrame'
    // Push student stud2
    //printf("El valor de concept es: %d y tiene un tamaño de %d bytes\n", stud2.concept, sizeof(stud2.concept));

    createFrame(stack);
    uint32_t *aux = (uint32_t*) stud2.califications;
    stack->push(stack, stud2.concept);
    stack->push(stack, aux[0]);
    stack->push(stack, stud2.dni);
    aux = (uint32_t*) stud2.name;
    for(int i=5; i>=0; i--){
        stack->push(stack, aux[i]);
    }


    // Push student stud1
    createFrame(stack);
    aux = (uint32_t*) stud1.califications;
    stack->push(stack, stud1.concept);
    stack->push(stack, aux[0]);
    stack->push(stack, stud1.dni);
    aux = (uint32_t*) stud1.name;
    for(int i=5; i>=0; i--){
        stack->push(stack, aux[i]);
    }

    // Push random value

    uint32_t value = 42;
    stack->push(stack,value);



    // Print student st1 y st2
    void (*prStudpt)() = printStudent;
    myCall(stack, prStudpt);

    // A qué apunta el esp???


    prStudpt = printStudentp;
    myCall(stack, prStudpt);

    free(stack->_stackMem);
    free(stack); // Alcanza?
    //creo que hay que hacer free del otro stack tambien

    return 0;
}
