//
// Created by pierre on 4/26/26.
//

#include "alloc.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>

typedef enum VALGRINDCHECK_METHOD {
    COUNT,
    FAIL_AT,
    FAIL_INCREMENTAL
} t_valgrindcheck_method;

struct valgrindcheck_settings {
    char valgrindcheck_alloc_filename[PATH_MAX + 1];
    char valgrindcheck_counter_filename[PATH_MAX + 1];
    int should_fail_at;
    t_valgrindcheck_method method;
};

static void valgrindcheck_init(struct valgrindcheck_settings *settings_ptr) {
    static int initialized = 0;
    struct valgrindcheck_settings settings = {0};
    char *tmp;

    if (initialized)
        return ;

    settings.should_fail_at = -1;
    tmp = getenv("VALGRINDCHECK_ALLOC_FILENAME");
    if (tmp)
        strcpy(settings.valgrindcheck_alloc_filename, tmp);
    else
        strcpy(settings.valgrindcheck_alloc_filename, "valgrindcheck.alloc");

    tmp = getenv("VALGRINDCHECK_COUNT_FILENAME");
    if (tmp)
        strcpy(settings.valgrindcheck_counter_filename, tmp);
    else
        strcpy(settings.valgrindcheck_counter_filename, "valgrindcheck.count");

    tmp = getenv("VALGRINDCHECK_FAIL_AT");
    if (tmp)
        settings.should_fail_at = atoi(tmp);

    if (getenv("VALGRINDCHECK_COUNT") && atoi(getenv("VALGRINDCHECK_COUNT")) == 1) {
        settings.method = COUNT;
    } else if (settings.should_fail_at != -1) {
        settings.method = FAIL_AT;
    } else {
        settings.method = FAIL_INCREMENTAL;
    }

    initialized = 1;
    *settings_ptr = settings;
}

typedef int (*t_valgrindcheck_method_ptr)(struct valgrindcheck_settings*);

/**
 * This method will count the total number of allocation call and save it in VALGRINDCHECK_COUNT_FILENAME file
 *
 * @param settings
 * @return 0
 */
static int valgrindcheck_count(struct valgrindcheck_settings *settings) {
    static int counter = 0;
    int fd = open(settings->valgrindcheck_counter_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        write(2, "valgrindcheck could not open file: ", strlen("valgrindcheck could not open file: "));
        write(2, strerror(errno), strlen(strerror(errno)));
        return 0;
    }
    ++counter;
    write(fd, &counter, sizeof(counter));
    close(fd);
    return 0;
}

/**
 * This method will succeed until the internal counter reach the VALGRINDCHECK_FAIL_AT environment variable
 *
 * @param settings
 * @return
 */
static int valgrindcheck_fail_at(struct valgrindcheck_settings *settings) {
    static int counter = 0;
    if (counter++ == settings->should_fail_at)
        return 1;
    return 0;
}

/**
 * This method will check the last call the last fail occurred and fail in the next one
 *
 * @param settings
 * @return
 */
static int valgrindcheck_fail_incremental(struct valgrindcheck_settings *settings) {
    static int initialized = 0;

    if (!initialized) {
        struct stat st;

        if (stat(settings->valgrindcheck_alloc_filename, &st)) {
            int fd = open(settings->valgrindcheck_alloc_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            settings->should_fail_at = 0;
            if (fd < 0) {
                write(2, "valgrindcheck could not open file: ", strlen("valgrindcheck could not open file: "));
                write(2, strerror(errno), strlen(strerror(errno)));
            }
            int tmp = 1;
            write(fd, &tmp, sizeof(tmp));
            close(fd);
        } else {
            int fd = open(settings->valgrindcheck_alloc_filename, O_RDWR);
            if (fd < 0) {
                const char *err = strerror(errno);
                write(2, "valgrindcheck could not open file: ", strlen("valgrindcheck could not open file: "));
                write(2, err, strlen(err));
            }
            if (read(fd, &settings->should_fail_at, sizeof(settings->should_fail_at)) != sizeof(settings->should_fail_at)) {
                settings->should_fail_at = 0;
            }

            int tmp = settings->should_fail_at + 1;
            pwrite(fd, &tmp, sizeof(tmp), 0);
            close(fd);
        }
        initialized = 1;
    }

    return valgrindcheck_fail_at(settings);
}

void *valgrindcheck_malloc(size_t size) {
    static struct valgrindcheck_settings settings;
    static t_valgrindcheck_method_ptr methods[] = {
        [COUNT] = valgrindcheck_count,
        [FAIL_AT] = valgrindcheck_fail_at,
        [FAIL_INCREMENTAL] = valgrindcheck_fail_incremental
    };
    valgrindcheck_init(&settings);

    if (methods[settings.method](&settings))
        return NULL;

    void *ptr = malloc(size);

    if (!ptr)
        return NULL;

    return ptr;
}

void valgrindcheck_free(void *ptr) {
    free(ptr);
}

