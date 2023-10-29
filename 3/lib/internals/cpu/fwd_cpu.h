#pragma once

#include "../mem/mem.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>

class CPU;
class ExecUnit;
class GPRegister;
class SPRegister;

using exectime_t = std::size_t; 

enum class State {
    FREE, 
    BUSY
};

using ExecUnits = std::vector<std::pair<State,ExecUnit>>;

using Mem = std::pair<std::unique_ptr<Data>, std::unique_ptr<SafeText>>;

Mem file_to_mcode (InstrSet &iset, const char *filename, std::vector <std::size_t> &bps);
