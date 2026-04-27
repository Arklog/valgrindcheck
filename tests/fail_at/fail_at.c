#include "libvalgrindcheck/alloc.h"

int main() {
    for (int i = 0; i < 10; ++i) {
        void *ptr = valgrindcheck_malloc(1024);

        if (ptr == NULL && FAIL_AT == i)
            return 0;
    }

    return 1;
}