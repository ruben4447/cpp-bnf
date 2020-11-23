# Basic BNF interpreter
BNF (Backus-Naur Form) is a metasyntax
This interpreter interprets basic BNF definitions from a file which may then have input tested against them

# Fail_Msgs
This is a pass-by-reference vector of strings which contains any error messages if the match/test should fail
Format: `(var [variable], group: [group]) error [msg]`

- variable
    - The variable the interpreter was matching against when the error occured
- group
    - The current group the interpreter was attempting to match agains (each group is seperated by `|`)
- msg
    - The error message

# Matches
This is a pass-by-reference vector of strings which contains any matches.
Any top-level variable is added to this vector (assuming success)