#pragma once
#include "../cmd_base/cmd_base.h"
#include <fstream>
#include <vector>


class MCode {

public:

    mprog prog = {};

    MCode () = default;
    MCode(mprog _prog) { prog = _prog; }
};

std::ostream &operator<<(std::ostream &os, const MCode &mc);

MCode &to_mcode (InstrSet &iset, const char *filename);
MCode &compile (InstrSet &iset, std::ifstream &is);
mprog &parser(InstrSet &iset, std::ifstream &is);