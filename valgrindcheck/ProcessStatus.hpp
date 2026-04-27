//
// Created by pierre on 4/27/26.
//

#ifndef VALGRINDCHECK__PROCESSSTATUS_HPP
#define VALGRINDCHECK__PROCESSSTATUS_HPP

namespace vcheck {
    class ProcessStatus {
    public:
        ProcessStatus() = default;

        int status;

        int exit_code() const;
        bool exited() const;
    };
} // vcheck

#endif //VALGRINDCHECK__PROCESSSTATUS_HPP
