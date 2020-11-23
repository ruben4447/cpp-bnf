#include <iostream>
#include <regex>
#include <vector>

#include "classes/token.cpp"
#include "utils.cpp"

namespace bnf {
    // FORMAT:: [var] ::= [expr] | [expr] ...

    // Consume any whitespace
    void consume_whitespace(std::string& str) {
        int start_pos = 0;
        while (start_pos < str.length()) {
            if (str[start_pos] == '\t' || str[start_pos] == ' ') {
                start_pos++;
            } else {
                break;
            }
        }

        str = str.substr(start_pos, str.length() - start_pos);
    }

    // Match a variable. Return match, or empty string if no match
    bool consume_var(std::string& input, std::string& var) {
        consume_whitespace(input);
        std::regex pattern("^<([A-Za-z][A-Za-z0-9_-]*)>");
        std::smatch matches;
        std::regex_search(input, matches, pattern);

        if (matches.size() == 0) {
            var = "";
            return false;
        } else {
            var = matches.str(1);
            input = input.substr(matches.str(0).length(), input.length());
            return true;
        }
    }

    // Tests if an equals sign
    bool consume_assign(std::string& input) {
        consume_whitespace(input);
        if (input[0] == ':' && input[1] == ':' && input[2] == '=') {
            input = input.substr(3, input.length());
            return true;
        } else {
            return false;
        }
    }

    bool consume_string(std::string& input, std::string& value) {
        consume_whitespace(input);
        value = "";
        char quote = input[0];
        if (quote != '"' && quote != '\'') return false;
        input.erase(0, 1);

        while (input.length() > 0) {
            if (input[0] == '\\' && input.length() >= 2) {
                // Escape next character
                value += input[1];
                input.erase(0, 2);
            } else if (input[0] == quote) {
                // Close string
                input.erase(0, 1);
                return true;
            } else {
                value += input[0];
                input.erase(0, 1);
            }
        }

        // STRING HAS FAILED
        input = quote + value + input;
        value = "";
        return false;
    }

    bool consume_expr(std::string& input, tokenlist_t& tokens, std::string& msg) {
        bool ok;
        std::string val;
        std::string acc_value = "";

        while (input.length() > 0) {
            if (input[0] == '|') {
                input.erase(0, 1);
                break;
            }

            if (input[0] == ' ' || input[0] == '\t') {
                input.erase(0, 1);
                continue;
            }

            // Variable?
            ok = consume_var(input, val);
            if (ok) {
                if (acc_value.length() > 0) {
                    tokens.push_back({acc_value, false});
                    acc_value = "";
                }

                tokens.push_back({ val, true });
                continue;
            }

            // String?
            if (input[0] == '"' || input[0] == '\'') {
                ok = consume_string(input, val);
                if (ok) {
                    if (acc_value.length() > 0) {
                        tokens.push_back({acc_value, false});
                        acc_value = "";
                    }

                    tokens.push_back({ val, false });
                    continue;
                } else {
                    std::string qs(1, input[0]);
                    msg = "bnf::consume_expr: SyntaxError: Unterminated string: expected " + qs + "..." + qs + ", got " + input;
                    return false;
                }
            }

            // Append as a constant value
            acc_value += input[0];
            input.erase(0, 1);
        }

        if (acc_value.length() > 0) {
            tokens.push_back({acc_value, false});
        }

        return true;
    }
};