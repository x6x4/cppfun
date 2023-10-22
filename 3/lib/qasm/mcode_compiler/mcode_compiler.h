#pragma once

#include "fwd_mcode_compiler.h"


class MCode {

    std::vector<Command*> cmds;

public:

    ~MCode ();
    
    MCode () = default;
    MCode (MCode&) = default;
    MCode (const MCode&) = default;
    MCode (MCode&&) = default;
    MCode &operator= (const MCode& _mc);
    MCode& operator= (MCode&&) = default;

    std::size_t num_lines ();
    Command* operator[] (std::size_t num) const;

    void add_cmd (Command* cmd);
    void print_mcode (std::ostream &os) const;
};


std::ostream &operator<<(std::ostream &os, const MCode &mc);

MCode file_to_mcode (InstrSet &iset, const char *filename);
