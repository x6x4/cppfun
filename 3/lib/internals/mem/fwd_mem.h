#pragma once

class SPRegister;
class DataCell;
class Memory;
#include "../IR/IR.h"
#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

using SafeText = std::vector<std::unique_ptr<Command>>;
using Data = std::vector<int>;

using Text = std::vector<Command*>;

struct NumberedLine {
    std::size_t num = 0;
    std::string line = "nop";

    NumberedLine() {};
    NumberedLine (std::size_t _num, std::string _line) : num (_num), line (_line) {};
};

using strings = std::vector<NumberedLine>;