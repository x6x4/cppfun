#pragma once
#include "../cmd_base/cmd_base.h"
#include <cstddef>
#include <fstream>
#include <memory>
#include <vector>


class MCode {

    std::vector<std::shared_ptr<Command>> cmds;

public:

    ~MCode () = default; //{ for (auto cmd : cmds) { delete cmd; cmd = nullptr; } cmds.clear(); };

    MCode () = default;
    MCode (MCode&) = default;
    MCode (MCode&&) = default;
    MCode& operator= (const MCode&) = default;
    MCode& operator= (MCode&&) = default;

    auto operator[] (std::size_t num) const { return cmds[num]; }

    void add_cmd (std::shared_ptr<Command>cmd) { cmds.push_back(cmd); }

    void print_mcode (std::ostream &os) const {
        for (auto cmd : cmds) os << *cmd << '\n';
    }
};


std::ostream &operator<<(std::ostream &os, const MCode &mc);

MCode file_to_mcode (InstrSet &iset, const char *filename);
