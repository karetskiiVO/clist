#include "clist.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

const char* fdump = "dump.html";
const char* gdump = "dmp.dot";

void listCtor (List* lst, const size_t len) {
    if (lst == NULL) {
        return;
    }

    if (lst->arr == NULL) {
        lst->arr = (ElemList*)malloc((len + 1) * sizeof(ElemList));

        lst->arr[0].next  = 0;
        lst->arr[0].prev  = 0;
        lst->arr[0].value = POISON;

        for (size_t i = 1; i < len + 1; i++) {
            lst->arr[i].next  = (i + 1) % (len + 1);
            lst->arr[i].prev  = -1;
            lst->arr[i].value = POISON;
        }
    } else if (len > lst->capacity) {
        lst->arr = (ElemList*)realloc(lst->arr, (len + 1) * sizeof(ElemList));

        if (lst->free == 0) {
            lst->free = lst->capacity + 1;
        } else {
            size_t buf = lst->free;
            while (lst->arr[buf].next != 0) {
                buf = lst->arr[buf].next;
            }
            lst->arr[buf].next = lst->capacity + 1;
        }

        for (size_t i = lst->capacity + 1; i < len + 1; i++) {  /// !!!!!!!!!!!!!!
            lst->arr[i].next  = (i + 1) % (len + 1);
            lst->arr[i].prev  = -1;
            lst->arr[i].value = POISON;
        }
    } else {
        return;
    }

    lst->capacity = len;
}

void listNew (List* lst) {
    lst->arr  = NULL;
    lst->free = 1;
    lst->size = 0;
    lst->capacity = 1;

    listCtor(lst, 1);
}

size_t listAdd (List* lst, const size_t pos, const Elem_t value) {
    if (pos < 0) {
        return EMPTY;
    }

    if (lst->arr[pos].prev == EMPTY) {
        return EMPTY;
    }

    if (lst->capacity <= lst->size) {
        listCtor(lst, 2 * lst->capacity);
    }

    size_t freebuf = lst->free;

    lst->free = lst->arr[freebuf].next;

    lst->arr[freebuf].value = value;
    lst->arr[freebuf].next  = lst->arr[pos].next;
    lst->arr[freebuf].prev  = pos;

    lst->arr[lst->arr[pos].next].prev = freebuf;
    lst->arr[pos].next = freebuf;

    lst->size++;

    return freebuf;    
}

Elem_t listRem (List* lst, const size_t pos) {
    if (pos <= 0) {
        return POISON;
    }

    if (lst->arr[pos].prev == EMPTY) {
        return POISON;
    }

    Elem_t buf = lst->arr[pos].value;

    lst->arr[lst->arr[pos].next].prev = lst->arr[pos].prev;
    lst->arr[lst->arr[pos].next].prev = lst->arr[pos].prev;

    lst->arr[pos].value = POISON;
    lst->arr[pos].next  = lst->free;
    lst->arr[pos].prev  = -1;

    lst->free = pos;

    lst->size--;

    return buf;
}

void listDump (List* lst) {
    /// make graphviz visualisation
    FILE* graph = fopen(gdump, "w");
    fprintf(graph, "digraph g {\n\t{\n\t\tnode [shape=record];\n\t\trankdir=LR\n");
    for (size_t i = 0; i <= lst->capacity; i++) {
        fprintf(graph, "\t\tstruct%ld [label=\"<id>Num: %ld | value: %lg | {<pr>prev: %ld| <nt>next: %ld}\"];\n",
                            i, i, lst->arr[i].value, lst->arr[i].prev, lst->arr[i].next);
    }

    fprintf(graph, "\t}\n");

    for (size_t i = 0; i < lst->capacity; i++) {
        fprintf(graph, "\tstruct%ld:id -> struct%ld:id[style=\"invis\" weight=\"1000\"]\n", i, i + 1);
    }

    for (size_t i = 0; i <= lst->capacity; i++) {
        if (lst->arr[i].prev != EMPTY) {
            fprintf(graph, "\tstruct%ld:nt -> struct%ld:pr\n", i, lst->arr[i].next);
            fprintf(graph, "\tstruct%ld:nt -> struct%ld:pr\n", lst->arr[i].prev, i);
        } else {
            fprintf(graph, "\tstruct%ld:nt -> struct%ld:pr\n", i, lst->arr[i].next);
        }
    }
    fprintf(graph, "\n}");

    fclose(graph);
}