//
// Created by pierre on 4/27/26.
//

#include "ProcessPool.hpp"

#include <sys/wait.h>

namespace vcheck {
    ProcessPool::ProcessPool(process_list &processes, size_t concurrent_processes, const std::optional<std::function<void (Process&)>> &callback): processes{processes}, concurrent_processes(concurrent_processes), process_map{}, callback(callback) {
        processes.reserve(concurrent_processes);
    }

    void ProcessPool::run() {
        for (auto proc = processes.begin(); proc != processes.end(); ++proc) {
            // start process when available space
            while (process_map.size() >= concurrent_processes) {
                wait_for_process();
            }

            proc->start();
            process_map[proc->getPid()] = proc;
        }

        while (!process_map.empty()) {
            wait_for_process();
        }
    }

    void ProcessPool::wait_for_process() {
        while (!process_map.empty()) {
            int status;
            auto pid = waitpid(0, &status, 0);

            if (process_map.contains(pid)) {
                auto process = process_map[pid];
                process->status().status = status;

                if (callback.has_value())
                    callback.value()(*process);
                process_map.erase(pid);
                break;
            }
        }
    }
} // vcheck