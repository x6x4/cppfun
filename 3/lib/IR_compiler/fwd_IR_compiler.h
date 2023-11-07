#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <sstream>
#include "../internals/operands/operands.h"
#include <stdexcept>
#include <utility>
#include <vector>


//  PARSING

//  removes insignificant lines
strings preproc_text (std::ifstream &is);
std::unique_ptr<Data> parse_data (std::ifstream &is, std::unordered_set<ID> &data_label_table);

std::unique_ptr<SafeText> parse_text(const InstrSet &iset, strings &vec, const std::unordered_set<ID> &data_label_table);
std::unique_ptr<Command> parse_cmd(const InstrSet &iset, std::string &cmd, std::size_t line_num, 
    const std::unordered_set<ID> &data_label_table, std::unordered_set<ID> &code_label_table);
int parse_dr(std::string &data_str, std::size_t line_num, std::unordered_set<ID> &data_label_table);

const ID& FindLabel (const std::unordered_set<ID> &label_table, const ID &id);
std::logic_error CE (const char *section, const char *error, std::size_t line_num);

Mem file_to_mcode (const InstrSet &iset, const char *filename);
Mem file_to_mcode (const InstrSet &iset, std::ifstream &is);
void load_cpu (CPU &cpu, const char *filename);

