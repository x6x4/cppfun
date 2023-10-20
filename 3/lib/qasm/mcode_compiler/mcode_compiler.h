#pragma once
#include "../cmd_base/cmd_base.h"
#include <cstddef>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <vector>


class MCode {

    std::vector<Command*> cmds;

public:

    ~MCode () {
        for (auto cmd : cmds) { delete cmd; cmd = nullptr; }
        cmds.clear();
    }

    MCode () = default;
    MCode (MCode&) = default;
    MCode (const MCode&) = default;
    MCode (MCode&&) = default;
    MCode &operator= (const MCode& _mc) {
        this->cmds = _mc.cmds;
        for (std::size_t i = 0; i < _mc.cmds.size(); i++) 
            this->cmds[i] = _mc.cmds[i]->clone();

        return *this;
    }
    MCode& operator= (MCode&&) = default;

    std::size_t num_lines () { return cmds.size() ;}

    auto operator[] (std::size_t num) const { 
        if (num < cmds.size()) return cmds[num]; 
        throw std::logic_error("Access out of bounds");
    }

    void add_cmd (Command* cmd) { cmds.push_back(cmd); }

    void print_mcode (std::ostream &os) const {
        for (auto cmd : cmds) os << *cmd << '\n';
    }
};


std::ostream &operator<<(std::ostream &os, const MCode &mc);

MCode file_to_mcode (InstrSet &iset, const char *filename);
