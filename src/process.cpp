#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

#include "utils.cpp"

namespace bnf {
    std::vector<std::string> _process_file(std::string filepath, std::vector<std::string>& include_files) {
        std::string contents = get_file_contents(filepath);
        auto lines = string_split_newlines(contents);
        std::vector<std::string> output_lines;

        for (int i = 0; i < lines.size(); ++i) {
            if (lines[i][0] == '#') continue; // Comment
            else if (lines[i].length() == 0) continue; // Empty line
            else if (lines[i].substr(0, 8) == "@include") { // Include
                std::string path = lines[i].substr(9);
                
                // Path already looked at?
                if (std::find(include_files.begin(), include_files.end(), path) != include_files.end()) {
                    throw std::runtime_error("nbf::_process_file: circular @include '" + path + "' (line " + std::to_string(i) + ")");
                }

                auto include_files_cpy = include_files;
                include_files_cpy.push_back(path);

                // System default?
                if (path[0] == '<') path = "src/bnf/" + path.substr(1, path.length() - 2);
                path += ".bnf";

                for (auto oline : _process_file(path, include_files_cpy)) {
                    output_lines.push_back(oline);
                }
            } else {
                output_lines.push_back(lines[i]);
            }
        }

        return output_lines;
    }

    // Process file and return definition lines
    std::vector<std::string> process_file(std::string filepath) {
        std::vector<std::string> include_files;
        return _process_file(filepath, include_files);
    }
};