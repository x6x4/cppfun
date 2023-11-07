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
using strings = std::vector<std::string>;