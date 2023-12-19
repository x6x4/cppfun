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
class NumberedCell;
class PCRegister;
class String;

using exectime_t = std::size_t; 

enum class State {
    FREE, 
    BUSY
};

using Mem = std::pair<std::unique_ptr<Data>, std::unique_ptr<SafeText>>;
