//
// Created by pierre on 4/27/26.
//

#ifndef VALGRINDCHECK__PROCESS_HPP
#define VALGRINDCHECK__PROCESS_HPP

#include <string>
#include <vector>

#include "ProcessStatus.hpp"
#include "Env.hpp"

namespace vcheck {
    class Process {
    public:
        using arglist = std::vector<std::string>;

        Process(const arglist &args, const Env& env);

        void start();

        void wait();

        pid_t getPid() const;

        ProcessStatus &status();

    private:
        const arglist                                 args;
        const Env                                     env;
        pid_t                                         pid;
        ProcessStatus                                 wstatus;
    };
} // vcheck

#endif //VALGRINDCHECK__PROCESS_HPP
