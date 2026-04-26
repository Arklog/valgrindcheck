//
// Created by pierre on 4/26/26.
//
#include <stdlib.h>

#include "libvalgrindcheck/alloc.h"

int main(int argc, char **argv) {
    void *ptr;
    long expected_counter = strtol(argv[1], NULL, 10);

    int i;
    for (i = 0; i < 5; ++i) {
        ptr = valgrindcheck_malloc(10);
        if (!ptr)
            break;
        valgrindcheck_free(ptr);
    }

    return !(expected_counter == i);
}