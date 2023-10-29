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


using strings = std::vector<std::string>;

strings preproc (std::ifstream &is);

Mem parser(InstrSet &iset, strings &vec, std::vector <std::size_t> &bps);
std::unique_ptr<Command> parse_cmd(InstrSet &iset, std::string &cmd, std::size_t line_num, 
                    std::unordered_set<ID> &data_label_table, std::unordered_set<ID> &code_label_table);
void parse_dr(std::string &data_str, std::size_t line_num, 
                    std::vector<int> &data, std::unordered_set<ID> &data_label_table);
std::logic_error CE (const char *section, const char *error, std::size_t line_num);

Mem file_to_mcode (InstrSet &iset, const char *filename, std::vector <std::size_t> &bps);
Mem file_to_mcode (InstrSet &iset, std::ifstream &is, std::vector <std::size_t> &bps);

