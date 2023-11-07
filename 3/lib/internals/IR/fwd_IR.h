#pragma once

#include <cstddef>
#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <vector>


class CPU_Cache;
class UnaryCommand;
class BinaryCommand;
using addr_t = std::size_t;
class UnaryOperator;
class BinaryOperator;
class CPU;
using Mnemonic = std::string;
class ExecUnit;