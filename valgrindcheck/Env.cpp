//
// Created by pierre on 4/27/26.
//

#include "Env.hpp"
#include <cstring>

namespace vcheck {
    Env::Env(): env{} {
        for (char **iter = environ; *iter != nullptr; ++iter) {
            std::string item(*iter);
            auto separator = item.find('=');
            env[item.substr(0, separator)] = item.substr(separator + 1);
        }
    }

    char ** Env::getenv() const {
        char **new_env = new char*[env.size() + 1];
        memset(new_env, 0, sizeof(char*) * (env.size() + 1));

        size_t i = 0;
        for (const auto& [key, value] : env) {
            char *new_item = new char[key.size() + value.size() + 2];
            strcpy(new_item, key.c_str());
            strcat(new_item, "=");
            strcat(new_item, value.c_str());
            new_env[i++] = new_item;
        }

        return new_env;
    }

    void Env::setenv(const char *name, const char *value) {
        env[name] = value;
    }
} // vcheck