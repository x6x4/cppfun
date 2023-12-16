#pragma once

#include <cstddef>
#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include "../../vector/libvec/vec.h"


class CPU_Cache;
class Command;
using addr_t = std::size_t;
class CPU;
using Mnemonic = std::string;
class ExecUnit;