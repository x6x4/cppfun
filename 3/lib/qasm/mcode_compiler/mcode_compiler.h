#pragma once
#include "../cmd_base/cmd_base.h"
#include <fstream>
#include <vector>


class MCode {

    std::vector<Command*> cmds;

public:

    ~MCode () { for (auto cmd : cmds) delete cmd; };

    MCode () = default;
    MCode (MCode&) = default;
    MCode (MCode&&) = default;
    MCode& operator= (MCode&) = default;
    MCode& operator= (MCode&&) = default;

    void add_cmd (Command *cmd) {
        cmds.push_back(cmd);
    }

    void print_mcode (std::ostream &os) const {
        for (auto cmd : cmds) os << *cmd << '\n';
    }
};


std::ostream &operator<<(std::ostream &os, const MCode &mc);

MCode file_to_mcode (InstrSet &iset, const char *filename);
