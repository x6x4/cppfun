#pragma once

#include <cstddef>
#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_set>


class CPU_Cache;
class UnaryCommand;
class BinaryCommand;
class TernaryCommand;
using addr_t = std::size_t;
class UnaryOperator;
class BinaryOperator;
class TernaryOperator;
class CPU;
using Mnemonic = std::string;
class ExecUnit;