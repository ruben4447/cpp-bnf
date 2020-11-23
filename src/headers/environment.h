#pragma once

#include <map>
#include <string>
#include "variable.h"

namespace bnf {
    class Environment {
        public:
            std::map<std::string, Variable> vars; // { name, var_object }

            void define_var_fatal(std::string def);
            bool define_var(std::string def, std::string& msg);
            void printvars();

            bool test(std::string& input, std::string var_name, std::vector<std::string>& fail_msgs);
            bool match(std::string &input, std::string var_name, std::vector<std::string> &fail_msgs, std::vector<std::string>& matches);
    };
};