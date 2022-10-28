#include "clist.h"

#include <stdio.h>

int main () {
    List list;
    listNew(&list);
    listDump(&list);
    
    for (size_t i = 0; i < 5; i++) {
        listAdd(&list, i, i);
        listDump(&list);
    }

    for (size_t i = 0; i < 5; i++) {
        listAdd(&list, 0, 10 * i);
        listDump(&list);
    }

    for (size_t i = 0; i < 5; i++) {
        listRem(&list, i);
        listDump(&list);
    }

    printf("%ld\n", listFind(&list, 9));

    listLin(&list);
    printf("%ld\n", listFind(&list, 2));
    listDump(&list);

    return 0;
}