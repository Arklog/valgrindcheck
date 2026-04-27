//
// Created by pierre on 4/27/26.
//

#include "ProcessStatus.hpp"

#include <stdexcept>

namespace vcheck {
    int ProcessStatus::exit_code() const {
        if (!exited())
            throw std::runtime_error("Process not exited");

        return WEXITSTATUS(status);
    }

    bool ProcessStatus::exited() const {
        return WIFEXITED(status);
    }
} // vcheck