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

using exectime_t = std::size_t; 

enum class State {
    FREE, 
    BUSY
};

using ExecUnits = std::vector<std::pair<State,ExecUnit>>;