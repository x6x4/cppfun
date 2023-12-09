#pragma once

#include "../mem/mem.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <fstream>
#include <iostream>

class CPU;
class ExecUnit;
class GPRegister;
class PCRegister;

using exectime_t = std::size_t; 

enum class State {
    FREE, 
    BUSY
};

using ExecUnits = my_std::Vec<std::pair<State,ExecUnit>>;
using Mem = std::pair<std::unique_ptr<Data>, std::unique_ptr<SafeText>>;
