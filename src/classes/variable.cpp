#include "../headers/variable.h"
#include <iostream>
#include <string>

bnf::Variable::Variable(std::string name) {
    bnf::Variable::_name = name;
    bnf::Variable::_is_const = false;
}

bnf::Variable::Variable(std::string name, bool is_constant) {
    bnf::Variable::_name = name;
    bnf::Variable::_is_const = is_constant;
}

std::string bnf::Variable::get_name() { return _name; }
bool bnf::Variable::is_constant() { return _is_const; }

void bnf::Variable::print() {
    for (int i = 0; i < groups.size(); i++) {
        std::cout << "~ Group " << (i + 1) << std::endl;
        std::cout << "  ~";
        for (auto pair : groups[i]) {
            std::cout << " ";
            if (pair.second) {
                std::cout << "<" << pair.first << ">";
            } else {
                std::cout << "`" << pair.first << "`";
            }
        }
        std::cout << std::endl;
    }
}

// Get a group (string)
std::string bnf::Variable::get_group(int index) {
    if (index < 0 || index >= groups.size()) {
        return "";
    } else {
        std::string str = "";
        for (auto token : groups[index]) {
            if (token.second) {
                str += "<" + token.first + "> ";
            } else {
                str += token.first + ' ';
            }
        }
        str.erase(str.length() - 1, 1);
        return str;
    }
}

// Return matches against variable; matches are top-level variables
bool bnf::Variable::match(std::string& input, bnf::Environment* env, std::vector<std::string>& fail_msgs, std::vector<std::string>& matches) {
    if (groups.size() == 0) throw std::runtime_error("bnf::Variable::test: error: empty variable '" + _name + "'");
    if (input.length() == 0) {
        fail_msgs.push_back("(var " + _name + ") error: input is empty");
        return false;
    }

    for (int i = 0; i < groups.size(); ++i) {
        if (groups[i].size() == 0) {
            fail_msgs.push_back("(var " + _name + ", group #" + std::to_string(i) + ") error: empty group");
            continue;
        } else if (groups[i][0].first == _name) {
            fail_msgs.push_back("(var " + _name + ", group: `" + get_group(i) + "`) error: first token referencing self (infinite recursion)");
            continue;
        }
        std::string input_cpy = input;

        bool matched = false;
        int pos = 0;
        for (int tokenlist_i = 0; tokenlist_i < groups[i].size(); ++tokenlist_i) {
            token_t token = groups[i][tokenlist_i];

            if (token.second) {
                // Test variable!
                auto found = (*env).vars.find(token.first);
                if (found == (*env).vars.end()) {
                    fail_msgs.push_back("(var " + _name + ", group: `" + get_group(i) + "`) error: cannot resolve name '" + token.first + "'");
                    matched = false;
                    break;
                } else {
                    int ofmlen = fail_msgs.size();
                    std::string string_before = input_cpy;

                    matched = found->second.test(input_cpy, env, fail_msgs);
                    if (matched) {
                        // Get matches string
                        int length_diff = string_before.length() - input_cpy.length();
                        std::string extracted = string_before.substr(0, length_diff);
                        matches.push_back(extracted);

                        pos += extracted.length();
                        std::string msg = "(var " + _name + ", group: `" + get_group(i) + ") matched var " + found->first;
                        // std::cout << msg << std::endl;
                    } else {
                        int prev_errs = fail_msgs.size() - ofmlen;
                        std::string msg = "(var " + _name + ", group: `" + get_group(i) + "`) error: expected variable " + found->first + " to be matched (see previous " + std::to_string(prev_errs) + " errors) at position " + std::to_string(pos);
                        // std::cout << msg << std::endl;
                        fail_msgs.push_back(msg);
                        break;
                    }
                }
            } else {
                std::string input_substr = input_cpy.substr(0, token.first.length());
                if (input_substr == token.first) {
                    matched = true;
                    pos += token.first.length();
                    input_cpy = input_cpy.substr(token.first.length());
                    std::string msg = "(var " + _name + ", group: `" + get_group(i) + "`) matched `" + token.first + "`";
                    // std::cout << msg << std::endl;
                } else {
                    std::string msg = "(var " + _name + ", group: `" + get_group(i) + "`) error: expected token `" + token.first + "`, got `" + input_substr + "` (position " + std::to_string(pos) + ")";
                    // std::cout << msg << std::endl;
                    fail_msgs.push_back(msg);
                    matched = false;
                    break;
                }
            }
        }

        if (matched) {
            input = input_cpy;
            // input = (pos == input_cpy.length()) ? "" : input_cpy.substr(pos);
            fail_msgs.clear();
            return true;
        }
    }

    return false;
}

// Does the input match to the variable
bool bnf::Variable::test(std::string& input, bnf::Environment* env, std::vector<std::string>& fail_msgs) {
    std::vector<std::string> matches;
    bool success = match(input, env, fail_msgs, matches);
    return success;
}