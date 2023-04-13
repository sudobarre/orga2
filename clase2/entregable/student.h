#ifndef STUDENT_H
#define STUDENT_H

#define NAME_LEN    21
#define NUM_CALIFICATIONS   3

#include "stack.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

extern stack_t *stack;

typedef struct student
{
    char name[NAME_LEN];
    uint32_t dni;
    uint8_t califications[NUM_CALIFICATIONS];
    int16_t concept;
} student_t;

typedef struct studentp
{
    char name[NAME_LEN];
    uint32_t dni;
    uint8_t califications[NUM_CALIFICATIONS];
    int16_t concept;
} __attribute__((packed)) studentp_t;

/* A definir en student.c */
void printStudent();
void printStudentp();

#endif