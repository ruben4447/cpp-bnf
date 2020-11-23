#include <iostream>
#include <string>

#include "src/classes/environment.cpp"
#include "src/process.cpp"

int main(int argc, char* argv[]) {
    // Pre-process file ito lines
    std::string def_file(argv[1]);
    auto def_lines = bnf::process_file(def_file);

    bnf::Environment env; // Everything is stored in an "environment"

    // Define variables
    for (auto line : def_lines) env.define_var_fatal(line);

    std::string subject(argv[2]); // What to test
    std::vector<std::string> fail_msgs; // Vector containing error messages (if fail)
    std::vector<std::string> matches; // Vector containing matches
    bool success = env.match(subject, "decl", fail_msgs, matches);

    std::cout << "Success: " << success << std::endl;
    std::cout << "Left over: `" << subject << "`" << std::endl;
    if (success) {
        std::cout << "Matches (" << matches.size() << "):" << std::endl;
        for (auto match : matches) std::cout << " ~$ `" << match << "`" << std::endl;
    } else {
        std::cout << "Fail Messages (" << fail_msgs.size() << "): " << std::endl;
        for (std::string msg : fail_msgs) std::cout << " ~$ " << msg << std::endl;
    }

    return 0;
}