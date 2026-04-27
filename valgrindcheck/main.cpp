#include <iostream>

#include "Env.hpp"
#include "Process.hpp"
#include <indicators/progress_bar.hpp>

#include "ProcessPool.hpp"

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
    size_t      concurrent_processes;
    int         valgrind_exitcode;

    settings() {
        count_filename       = "count.bin";
        concurrent_processes = 5;
        valgrind_exitcode    = 42;
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
    }

    indicators::ProgressBar progress_bar{};
    vcheck::ProcessPool pool{processes, s.concurrent_processes, [&progress_bar, &processes](vcheck::Process &process) {
        static int n = 0;
        auto current = static_cast<float>(++n);
        auto max = static_cast<float>(processes.size());
        progress_bar.set_progress((current / max) * 100.0f);
    }};

    pool.run();

    i = 0;
    for (auto &process : processes) {
        auto status = process.status();

        if (status.exited() && status.exit_code() == 42) {
            std::cout << "process: " << i << std::endl;
        }
        ++i;
    }


    return 0;
}