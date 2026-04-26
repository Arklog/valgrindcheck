//
// Created by pierre on 4/26/26.
//

#include "alloc.h"

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <valgrind/memcheck.h>

#ifndef VALGRINDCHECK_ALLOC_FILENAME
#define VALGRINDCHECK_ALLOC_FILENAME alloc.valgrindcheck
#endif

#define STRINGIFY(exp) #exp
#define VALGRINDCHECK_ALLOC_FILENAME_STR STRINGIFY(VALGRINDCHECK_ALLOC_FILENAME)

static int load_from_env(int *should_fail_at) {
    const char *load_from_env = getenv("VALGRINDCHECK_FAIL_AT");

    if (load_from_env) {
        *should_fail_at = atoi(load_from_env);
        return 1;
    }

    return 0;
}

static int alloc_should_fail() {
    static int should_fail_at = -1;
    static int counter = 0;

    if (should_fail_at == -1 && !load_from_env(&should_fail_at)) {
        struct stat st;

        if (stat(VALGRINDCHECK_ALLOC_FILENAME_STR, &st)) {
            perror("valgrindcheck could not stat file: " VALGRINDCHECK_ALLOC_FILENAME_STR);
            should_fail_at = 0;
        } else {
            FILE *file = fopen(VALGRINDCHECK_ALLOC_FILENAME_STR, "r");
            if (!file) {
                perror("valgrindcheck could not open file: ");
            }

            if (fscanf(file, "%d", &should_fail_at) != 1) {
                should_fail_at = 0;
            }
            fclose(file);
        }

        FILE *file = fopen(VALGRINDCHECK_ALLOC_FILENAME_STR, "w");
        if (!file)
            perror("valgrindcheck could not open file: ");
        fprintf(file, "%d", should_fail_at + 1);
        fclose(file);
    }

    if (counter++ == should_fail_at)
        return 1;

    return 0;
}

void *valgrindcheck_malloc(size_t size) {
    if (alloc_should_fail())
        return NULL;

    void *ptr = malloc(size);

    if (!ptr)
        return NULL;

    // VALGRIND_MALLOCLIKE_BLOCK(ptr, size, 0, 0);

    return ptr;
}

void valgrindcheck_free(void *ptr) {
    // VALGRIND_FREELIKE_BLOCK(ptr, 0);

    free(ptr);
}

