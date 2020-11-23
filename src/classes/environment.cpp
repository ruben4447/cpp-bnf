#include <string>
#include <vector>

#include "../headers/environment.h"
#include "../classes/variable.cpp"
#include "../classes/token.cpp"
#include "../consume.cpp"

// Given a BNF var definition, define and create a Variable object
bool bnf::Environment::define_var(std::string def, std::string& msg) {
    bool ok, is_constant = false;

    if (def.substr(0, 5) == "const") {
        is_constant = true;
        def.erase(0, 5);
    }

    // Try extracting a variable
    std::string var_name;
    ok = bnf::consume_var(def, var_name);
    if (!ok) {
        msg = "bnf::Environment::define_var: SyntaxError: expected variable, got '" + def + "'";
        return false;
    } 

    // Define variable object
    Variable var_obj(var_name, is_constant);

    // Cannot overwrite constant!
    auto var_loc = bnf::Environment::vars.find(var_name);
    if (var_loc != bnf::Environment::vars.end() && var_loc->second.is_constant()) {
        msg = "bnf::Environment::define_var: NameError: Cannot redefine constant '" + var_name + "'";
        return false;
    }
    
    // Equals sign?
    ok = bnf::consume_assign(def);
    if (!ok) {
        msg = "bnf::Environment::define_var: SyntaxError: expected assignation operator, got '" + def + "'";
        return false;
    }

    while (def.length() > 0) {
        tokenlist_t tokens;
        ok = bnf::consume_expr(def, tokens, msg);
        if (ok) {
            var_obj.groups.push_back(tokens);
        } else
            return false;
    }

    auto found = bnf::Environment::vars.find(var_name);
    if (found == bnf::Environment::vars.end()) {
        bnf::Environment::vars.insert({ var_name, var_obj });
    } else {
        if (is_constant) {
            // Cannot make variable a consant variable after defined
            msg = "bnf::Environment::define_var: SyntaxError: unexpected keyword 'const'";
            return false;
        }
        found->second = var_obj;
    }

    msg = "bnf::Environment::define_var: Created variable '" + var_name + "' successfully";
    return true;
}

// Same as define_var, but exit program upon error
void bnf::Environment::define_var_fatal(std::string def) {
    std::string msg;
    if (!define_var(def, msg)) throw std::runtime_error(msg);
}

void bnf::Environment::printvars() {
    for (auto var : vars) {
        std::cout << "---- <" << var.first << "> ----" << std::endl;
        var.second.print();
    }
}

// Test some text against a variable
bool bnf::Environment::test(std::string& input, std::string var_name, std::vector<std::string>& fail_msgs) {
    // Does var exist?
    auto found = bnf::Environment::vars.find(var_name);
    if (found == bnf::Environment::vars.end()) {
        throw std::runtime_error("bnf::Environment::test: NameError: name " + var_name + " could not be resolved to a variable");
    }

    return found->second.test(input, this, fail_msgs);
}

// Get matches against a variable (matches: top-level variables)
bool bnf::Environment::match(std::string& input, std::string var_name, std::vector<std::string>& fail_msgs, std::vector<std::string>& matches) {
    // Does var exist?
    auto found = bnf::Environment::vars.find(var_name);
    if (found == bnf::Environment::vars.end()) {
        throw std::runtime_error("bnf::Environment::match: NameError: name " + var_name + " could not be resolved to a variable");
    }

    return found->second.match(input, this, fail_msgs, matches);
} 