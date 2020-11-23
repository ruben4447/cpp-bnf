#include <string>
#include <vector>

namespace bnf {
    // { value, is_var }
    typedef std::pair<std::string, bool> token_t;
    typedef std::vector<token_t> tokenlist_t;
};