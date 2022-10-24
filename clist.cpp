#include "clist.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void listCtor (List* lst, const size_t len) {
    if (lst == NULL) {
        // error
    }
    printf("%p\t%ld\n", lst, len);

    if (lst->arr == NULL) {
        lst->arr = (ElemList*)malloc(len * sizeof(ElemList));

        lst->free = 1;
        lst->size = 0;

        lst->arr[0].value = POISON;
        lst->arr[0].next  = 0;
        lst->arr[0].prev  = 0;

        for (size_t i = 1; i < len; i++) {
            lst->arr[i].value = POISON;
            lst->arr[i].next  = (i + 1) % len;
            lst->arr[i].prev  = -1;
        }
    } else {
        size_t capacitybuf = lst->capacity;

        lst->arr = (ElemList*)realloc(lst->arr, len * sizeof(ElemList));

        if (lst->free == 0) {
            lst->free = capacitybuf;
        } else {
            size_t lastfree = lst->free;
            while (lst->arr[lastfree++].next);

            ////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            lst->arr[lastfree].next = capacitybuf;
        }

        for(size_t i = capacitybuf ; i < len; i++) {
            lst->arr[i].value = POISON;
            lst->arr[i].next  = (i + 1) % len;
            lst->arr[i].prev  = -1;
        }
    }

    lst->capacity = len;
}

void listNew (List* lst) {
    //lst->arr = NULL;
    listCtor(lst, 1);
}

size_t listAdd (List* lst, const size_t pos, const Elem_t value) {
    if (lst->size < pos) {
        return -1;
        // error
    }

    size_t truepos = lst->arr[0].next;
    for (size_t i = 0; i < pos; i++) {
        truepos = lst->arr[truepos].next;
    }
    
    if (lst->free == 0) {
        listCtor(lst, 2 * lst->capacity);
    }

    size_t freebuf = lst->free;
    lst->free = lst->arr[freebuf].next;

    lst->arr[freebuf].prev = lst->arr[truepos].prev;
    lst->arr[freebuf].next = lst->arr[lst->arr[freebuf].prev].next;
    lst->arr[freebuf].value = value;

    lst->arr[lst->arr[freebuf].next].prev = truepos;
    lst->arr[lst->arr[freebuf].prev].next = truepos;

    lst->size++;

    return truepos;
}

Elem_t listRem (List* lst, const size_t pos) {
    if (lst->size < pos) {
        return POISON;
        // error
    }

    size_t truepos = lst->arr[0].next;
    for (size_t i = 0; i < pos; i++) {
        truepos = lst->arr[truepos].next;
    }

    Elem_t out = lst->arr[truepos].value;

    lst->arr[lst->arr[truepos].prev].next = lst->arr[truepos].next;
    lst->arr[lst->arr[truepos].next].prev = lst->arr[truepos].prev;

    lst->arr[truepos].prev = -1;
    lst->arr[truepos].next = lst->free;
    lst->arr[truepos].value = POISON;

    lst->free = truepos;   

    lst->size--;

    return out; 
}

void listDump (List* lst) {
    for (size_t i = 0; i < lst->capacity; i++) {
        printf("\t[%d]: %lg\t%ld\t%ld\n", i, lst->arr[i].value, lst->arr[i].prev, lst->arr[i].next);
    }
}
