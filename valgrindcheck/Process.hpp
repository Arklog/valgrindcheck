//
// Created by pierre on 4/27/26.
//

#ifndef VALGRINDCHECK__PROCESS_HPP
#define VALGRINDCHECK__PROCESS_HPP

#include <string>
#include <vector>


namespace vcheck {
    class Env;

    class Process {
    public:
        using arglist = std::vector<std::string>;

        Process(const arglist &args, const Env& env);

        void start();

        void wait();
    private:
        const arglist &args;
        const Env &env;
        pid_t     pid;
        int       status;
    };
} // vcheck

#endif //VALGRINDCHECK__PROCESS_HPP
