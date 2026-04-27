//
// Created by pierre on 4/27/26.
//

#ifndef VALGRINDCHECK__ENV_HPP
#define VALGRINDCHECK__ENV_HPP

#include <string>
#include <unordered_map>

extern "C" {
extern char **environ;
}

namespace vcheck {
    class Env {
    public:
        Env();

        char **getenv() const;

        void setenv(const char *name, const char *value);

    private:
        using map_type = std::unordered_map<std::string, std::string>;
        map_type env;
    };
} // vcheck

#endif //VALGRINDCHECK__ENV_HPP
