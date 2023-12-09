#pragma once

class PCRegister;
class DataCell;
class Memory;
#include "../IR/IR.h"
#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>
#include "../../vector/libvec/vec.h"

using SafeText = my_std::Vec<std::unique_ptr<Command>>;
using Data = my_std::Vec<int>;

using Text = my_std::Vec<Command*>;

struct NumberedLine {
    std::size_t num = 0;
    std::string line = "nop";

    NumberedLine() {};
    NumberedLine (std::size_t _num, std::string _line) : num (_num), line (_line) {}; 
};

std::ostream &operator<<(std::ostream &os, NumberedLine &nl);

using strings = my_std::Vec<NumberedLine>;