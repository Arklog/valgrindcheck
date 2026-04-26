//
// Created by pierre on 4/26/26.
//

#ifndef VALGRINDCHECK__ALLOC_H
#define VALGRINDCHECK__ALLOC_H
#include <stddef.h>

void *valgrindcheck_malloc(size_t size);
void valgrindcheck_free(void *ptr);

#endif //VALGRINDCHECK__ALLOC_H
