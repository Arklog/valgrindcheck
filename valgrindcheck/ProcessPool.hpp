//
// Created by pierre on 4/27/26.
//

#ifndef VALGRINDCHECK__PROCESSPOOL_HPP
#define VALGRINDCHECK__PROCESSPOOL_HPP
#include <vector>
#include <functional>
#include <optional>

#include "Process.hpp"

namespace vcheck {
    class ProcessPool {
    public:
        using process_list = std::vector<Process>;
        ProcessPool(process_list &processes, size_t concurrent_processes, const std::optional<std::function<void (Process &)>> & = {});

        void run();
    private:
        using process_lookup = std::unordered_map<pid_t, process_list::iterator>;

        process_list &processes;
        process_lookup process_map;
        size_t concurrent_processes;
        std::optional<std::function<void (Process&)>> callback;

        void wait_for_process();
    };
} // vcheck

#endif //VALGRINDCHECK__PROCESSPOOL_HPP
