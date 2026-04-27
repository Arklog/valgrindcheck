#include <iostream>

#include "Env.hpp"
#include "Process.hpp"

extern "C" {
#include <unistd.h>
#include <fcntl.h>
}

const vcheck::Process::arglist valgrind_args = {
    "/usr/bin/valgrind",
    "--error-exitcode=42",
    "--show-leak-kinds=all",
    "--track-origins=yes",
    "--leak-check=full"
};

struct settings {
    std::string count_filename;

    settings() {
        count_filename = "count.bin";
    }
};

int main(int argc, char **argv) {
    vcheck::Env env{};
    settings s;

    env.setenv("VALGRINDCHECK_COUNT", "1");
    env.setenv("VALGRINDCHECK_COUNT_FILENAME", "count.bin");
    vcheck::Process::arglist args{};

    for (int i = 1; i < argc; ++i) {
        args.push_back(argv[i]);
    }
    vcheck::Process process{args, env};
    process.start();
    process.wait();

    int i = 0;
    int fd = open(s.count_filename.c_str(), O_RDONLY);
    read(fd, &i, sizeof(i));
    close(fd);

    vcheck::Env child_env{};
    std::vector<vcheck::Process> processes;

    for (int n = 0; n < i; ++n) {
        auto child_args = valgrind_args;
        child_args.push_back("--log-file=" + std::to_string(n) + ".log");
        child_args.insert_range(child_args.end(), args);

        child_env.setenv("VALGRINDCHECK_FAIL_AT", std::to_string(n).c_str());
        vcheck::Process child{child_args, child_env};
        processes.push_back(child);
        child.start();
    }

    size_t n = 0;
    for (auto &p : processes) {
        p.wait();
        auto status = p.status();

        if (status.exited() && status.exit_code() == 42) {
            std::cout << "Test failed at " << n << std::endl;
        }
        ++n;
    }

    return 0;
}