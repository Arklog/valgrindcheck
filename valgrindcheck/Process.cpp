//
// Created by pierre on 4/27/26.
//

#include "Process.hpp"
#include "Env.hpp"

#include <cstring>
#include <stdexcept>
#include <spawn.h>
#include <sys/wait.h>

namespace vcheck {
    Process::Process(const arglist &args, const Env &env, int valgrindcheck_id) : args(args), env(env),
        valgrindcheck_id{valgrindcheck_id}, pid{}, wstatus{} {
    }

    void Process::start() {
        char **argv    = new char *[args.size() + 1];
        auto   raw_env = this->env.getenv();
        memset(argv, 0, sizeof(char *) * (args.size() + 1));

        size_t i = 0;
        for (const auto &arg: args) {
            argv[i++] = const_cast<char *>(arg.c_str());
        }

        if (posix_spawn(&pid, argv[0], nullptr, nullptr, argv, raw_env))
            throw std::runtime_error("posix_spawn failed: " + std::string(strerror(errno)));

        delete [] argv;
        for (auto iter = raw_env; *iter != nullptr; ++iter)
            delete [] *iter;
        delete [] raw_env;
    }

    void Process::wait() {
        if (waitpid(pid, &wstatus.status, 0) < 0)
            throw std::runtime_error("waitpid failed");
    }

    pid_t Process::getPid() const {
        return pid;
    }

    ProcessStatus &Process::status() {
        return wstatus;
    }

    int Process::getValgrindcheckId() const {
        return valgrindcheck_id;
    }
} // vcheck
