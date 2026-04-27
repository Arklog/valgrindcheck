//
// Created by pierre on 4/27/26.
//
#include <fcntl.h>
#include <unistd.h>

#include "libvalgrindcheck/alloc.h"

int main() {
    for (int i = 0; i < 10; i++) {
        valgrindcheck_malloc(1024);
    }

    int fd = open("test.out", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int n = 10;
    write(fd, &n, sizeof(n));
    close(fd);
    return 0;
}