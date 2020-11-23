# Files
Not monitored, but suggested to end in '.bnf' extension

Files are pre-processed using the function `bnf::process_file(...)`:
- Empty lines are ignored
- Lines starting with a `#` will be ignored (like a comment)
- `@include` statements insert a file directly into this one.    
    - Anything after `@include` will be treated as a file path
    e.g. *`@include integer` will insert the (pre-processed) contents of `./integer.bnf`*
            
    - A path surrounded in `<` and `>` will be treated as a standard library and be looked for in the relevant location
   e.g. *`@include <std>` will look for `./src/bnf/std.bnf` (only works if in same directory as `src` folder in this repo)*

# Variables:
Syntax: `<name>`
- Must be surrounded by `<` and `>`
- `name` must start with a letter
- `name` can contain letters, numbers, dashes and underscores

# Assignment
Assign something to a variable   
Syntax: `[const] <var> ::= expr | expr ...`

- Flag `const` is optional and will define the variable as a constant - may not be overwritten
- Each bar `|` seperates an expression, and acts as an OR operation. NOTE that if a expr is matched, evaluation will stop

# Expressions
- May contain strings surrounded by " (double quotes) or ' (single quotes)
    - If strings are not terminated (i.e. only one quote mark), it will be treated as a character to-match
    - EVERYTHING inside a string must be matched 100%
- Variables inside expressions will be looked up recursively
- Spaces are ignored (so for a literal space, surround it in quotes)

If using the match() function, any top-level variables will be captured