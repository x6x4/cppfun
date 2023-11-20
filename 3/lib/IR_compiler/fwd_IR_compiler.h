#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <sstream>
#include "../internals/cpu/cpu.h"
#include <stdexcept>
#include <utility>


//  PARSING

strings to_strings (std::istream &is);

//  removes insignificant lines
std::pair<strings, strings> preproc_code (strings program);
std::unique_ptr<Data> parse_data (strings program, std::unordered_set<ID> &data_label_table);

std::unique_ptr<SafeText> parse_text(const InstrSet &iset, strings &vec, const std::unordered_set<ID> &data_label_table);
std::unique_ptr<Command> parse_cmd(const InstrSet &iset, NumberedLine cmd_str, 
    const std::unordered_set<ID> &data_label_table, std::unordered_set<ID> &code_label_table);
int parse_dr(NumberedLine data_str, std::unordered_set<ID> &data_label_table);


const ID& FindLabel (const std::unordered_set<ID> &label_table, const ID &id);
std::logic_error CE (const char *section, const char *error, std::size_t line_num);

Mem file_to_mcode (const InstrSet &iset, strings program);
strings load_file_cpu (CPU &cpu, const std::string &filename);
strings load_text_cpu (CPU &cpu, const std::string &program_text);

