#include "student.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


void printStudent()
{
    printf("Valor: %d\n", stack->pop(stack));
    printf("Nombre: ");
    for(int i=0; i<6; i++){
        uint32_t aux = stack->pop(stack);
        printf("%s", &aux);
    }
    printf("\n");
    printf("dni: %d\n", stack->pop(stack));
    uint32_t calificaciones = stack->pop(stack);
    printf("Calificaciones: ");
    for(int i = 0; i<3; i++) {
        printf("%d, ", (uint8_t) calificaciones);
        calificaciones = calificaciones>>8;
    }
    printf("\nConcepto: %d\n", stack->pop(stack));
    printf("-------------\n");
}

void printStudentp()
{
    printf("Nombre: ");
    for(int i=0; i<6; i++){
        uint32_t aux = stack->pop(stack);
        printf("%s", &aux);
    }
    printf("\n");
    printf("dni: %d\n", stack->pop(stack));
    uint32_t calificaciones = stack->pop(stack);
    printf("Calificaciones: ");
    for(int i = 0; i<3; i++) {
        printf("%d, ", (uint8_t) calificaciones);
        calificaciones = calificaciones>>8;
    }
    printf("\nConcepto: %d\n", stack->pop(stack));
    printf("-------------\n");
}
