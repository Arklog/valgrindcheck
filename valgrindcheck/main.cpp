#include <iostream>

#include "Env.hpp"
#include "Process.hpp"
#include "ProcessPool.hpp"

#include <indicators/progress_bar.hpp>
#include <args.hxx>


extern "C" {
#include <unistd.h>
#include <fcntl.h>
}

vcheck::Process::arglist valgrind_args = {
    "/usr/bin/valgrind",
    "--show-leak-kinds=all",
    "--track-origins=yes",
    "--leak-check=full"
};

struct settings {
    std::string              count_filename;
    size_t                   concurrent_processes;
    int                      valgrind_exitcode;
    vcheck::Process::arglist child_args;

    settings() : child_args{} {
        count_filename       = "count.bin";
        concurrent_processes = 20;
        valgrind_exitcode    = 42;
    }
};

static void parse_args(int argc, char **argv, settings &s) {
    args::ArgumentParser parser{""};

    args::HelpFlag          help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlag<size_t> concurrent_processes(parser, "concurrent_processes",
                                                 "Maximum number of concurrent processes",
                                                 {'c', "concurrent"}, 10);
    args::ValueFlag<int> valgrind_exitcode(parser, "valgrind_exitcode", "Valgrind exit code", {'e', "exitcode"}, 42);
    args::PositionalList<std::string> child_args(parser, "COMMAND", "The program to check");

    try {
        parser.ParseCLI(argc, argv);
    } catch (const args::Help &) {
        std::cout << parser;
        exit(0);
    }

    s.concurrent_processes = args::get(concurrent_processes);
    s.valgrind_exitcode    = args::get(valgrind_exitcode);
    s.child_args           = args::get(child_args);
}

int main(int argc, char **argv) {
    vcheck::Env env{};
    settings    s;
    parse_args(argc, argv, s);

    valgrind_args.push_back("--error-exitcode=" + std::to_string(s.valgrind_exitcode));
    env.setenv("VALGRINDCHECK_COUNT", "1");
    env.setenv("VALGRINDCHECK_COUNT_FILENAME", "count.bin");

    vcheck::Process process{s.child_args, env};
    process.start();
    process.wait();

    int i  = 0;
    int fd = open(s.count_filename.c_str(), O_RDONLY);
    read(fd, &i, sizeof(i));
    close(fd);

    vcheck::Env                  child_env{};
    std::vector<vcheck::Process> processes;

    for (int n = 0; n < i; ++n) {
        auto child_args = valgrind_args;
        child_args.push_back("--log-file=" + std::to_string(n) + ".log");
        child_args.insert_range(child_args.end(), s.child_args);

        child_env.setenv("VALGRINDCHECK_FAIL_AT", std::to_string(n).c_str());
        vcheck::Process child{child_args, child_env, n};
        processes.push_back(child);
    }

    indicators::ProgressBar progress_bar{
        indicators::option::MaxProgress{processes.size()},
        indicators::option::ShowPercentage{true},
        indicators::option::ShowRemainingTime{true}
    };

    std::vector<decltype(processes)::iterator> failed_processes;
    vcheck::ProcessPool                        pool{
        processes, s.concurrent_processes,
        [&progress_bar, &failed_processes, &s](decltype(processes)::iterator process) {
            static int n = 0;
            progress_bar.tick();
            if (process->status().exited() && process->status().exit_code() == s.valgrind_exitcode)
                failed_processes.push_back(process);
        }
    };

    pool.run();

    for (auto process: failed_processes) {
        std::cout << "leak detected: " << process->getValgrindcheckId() << std::endl;
    }

    return failed_processes.empty() ? 0 : 1;
}
