#pragma once

#include <vector>
#include <string>
#include "../classes/token.cpp"

namespace bnf {
    class Environment;

    class Variable {
        private:
            std::string _name;
            bool _is_const;

        public:
            std::vector<tokenlist_t> groups; // List of lists of token pairs

            Variable(std::string name);
            Variable(std::string name, bool is_constant);

            std::string get_name();
            bool is_constant();
            std::string get_group(int index);

            bool test(std::string& input, Environment* env, std::vector<std::string>& fail_msgs);
            bool match(std::string &input, Environment *env, std::vector<std::string>& fail_msgs, std::vector<std::string>& matches);

            void print();
    };
};