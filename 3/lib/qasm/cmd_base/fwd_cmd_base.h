#pragma once

#include <cstddef>
#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <vector>


class Cache;
class UnaryCommand;
class BinaryCommand;
using addr_t = std::size_t;
class UnaryOperator;
class BinaryOperator;
using unary_instr_set = std::unordered_set<UnaryOperator>;
using binary_instr_set = std::unordered_set<BinaryOperator>;
class CPU;
using Mnemonic = std::string;
class ExecUnit;