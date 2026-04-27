#include "Env.hpp"
#include <spawn.h>
#include <sys/wait.h>

struct settings {
};

int main(int argc, char **argv) {
    vcheck::Env env{};

    env.setenv("VALGRINDCHECK_COUNT", "1");
}