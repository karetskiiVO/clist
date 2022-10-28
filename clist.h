#ifndef CLIST
#define CLIST

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

/**
 * @brief Elem_t   - type of stack elements
 * @brief Elem_out - flag to scanf
 * @brief POISON   - empty element
 */
typedef double Elem_t;
const Elem_t POISON = (Elem_t)INFINITY;
const size_t EMPTY  = (size_t)(-1);

typedef struct {
    Elem_t value;
    size_t next;
    size_t prev;
} ElemList;

typedef struct {
    ElemList* arr;
    size_t size;
    size_t capacity;
    size_t free;
    bool fastfind;
} List;

void listCtor (List* lst, const size_t len);

void listNew (List* lst);

size_t listAdd (List* lst, const size_t pos, const Elem_t value);

Elem_t listRem (List* lst, const size_t pos);

void listDump (List* lst);

void listLin  (List* lst);

#endif // clist.h