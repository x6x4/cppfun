
#include "fwd_IR_compiler.h"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fmt/core.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <utility>
#include "../../lib/internals/operands/operands.h"


// MAIN

strings to_strings (std::istream &is) {
    strings vec;
        
    std::string line;
    std::size_t user_line_num = 0;
        
    while (std::getline(is, line)) 
        vec.push_back(NumberedLine(user_line_num++, line));

    return vec;
}

strings load_text_cpu (CPU &cpu, const std::string &program_text, my_std::Vec<std::size_t> &avl_bps) {
    std::istringstream iss(program_text);
    strings program = to_strings(iss);

    cpu.load_mem(file_to_mcode(cpu.iSet(), program, avl_bps));
    return program;
}

strings load_file_cpu (CPU &cpu, const std::string &filename, my_std::Vec<std::size_t> &avl_bps) {
    try {
        std::ifstream iss(filename);
        if (iss.fail()) throw std::logic_error ("Wrong file");
        strings program = to_strings(iss);

        cpu.load_mem(file_to_mcode(cpu.iSet(), program, avl_bps));
        return program;
    }
    catch (std::runtime_error &e) {
        std::cout << __PRETTY_FUNCTION__ << ": " << '\n';
        throw e;
    }
    catch (std::logic_error &e) {
        std::cout << __PRETTY_FUNCTION__ << ": " << '\n';
        throw e;
    }
}

Mem file_to_mcode (const InstrSet &iset, strings program, my_std::Vec<std::size_t> &avl_bps) {

    std::unordered_set<ID> data_label_table;

    try {
        auto vec_pair = preproc_code(program, avl_bps);
        std::unique_ptr<Data> data = parse_data(vec_pair.second, data_label_table);
        std::unique_ptr<SafeText> text = parse_text(iset, vec_pair.first, data_label_table);

        return std::make_pair(std::move(data), std::move(text));
    }

    catch (std::runtime_error &e) {
        std::cout << __PRETTY_FUNCTION__ << ": " << '\n';
        throw e;
    }
    catch (std::logic_error &e) {
        std::cout << __PRETTY_FUNCTION__ << ": " << '\n';
        throw e;
    }
}

//  PREPROC AND SECTIONS PARSING

std::pair<strings, strings> preproc_code (strings program, my_std::Vec<std::size_t> &avl_bps) {

    strings vec_text, vec_data;
    std::string line;
    bool is_data = 0;
        
    for (auto asm_line : program) {
        if (!asm_line.line.compare(".text")) continue;    //  start of .text section
        if (!asm_line.line.compare("")) continue;    //  empty line
        if (asm_line.line[0] == '#') continue;       //  comment
        if (!asm_line.line.compare(".data")) { is_data = 1; continue; } //  .data section

        if (!is_data) {
            vec_text.push_back(asm_line);
            avl_bps.push_back(asm_line.num);
        } else  
            vec_data.push_back(asm_line);
    }

    return std::make_pair(vec_text, vec_data);
}



std::unique_ptr<SafeText> parse_text(const InstrSet &iset, strings &vec_text, const std::unordered_set<ID> &data_label_table) {

    std::unique_ptr<Command> cur_cmd;
    std::unordered_set<ID> code_label_table;
    std::size_t cur_num = 0;

    std::unique_ptr<SafeText> text = std::make_unique<SafeText>(SafeText());
 
    try {
        for (auto cmd : vec_text) {  //  line-by-line parsing
            cur_num = cmd.num + 1;
            cur_cmd = parse_cmd(iset, cmd, data_label_table, code_label_table);
            text->push_back(std::move(cur_cmd));
        }
    }

    catch (std::runtime_error &e) {
        throw CE(".text", e.what(), cur_num);
    }
    catch (std::logic_error &e) {
        throw CE(".text", e.what(), cur_num);
    }

    return text;
}

std::unique_ptr<Data> parse_data (strings vec_data, std::unordered_set<ID> &data_label_table) {

    std::unique_ptr<Data> data = std::make_unique<Data>(Data());    
    std::size_t rel_num = 0;
    std::size_t abs_num = 0;

    try {
        for (auto cur_directive : vec_data) {  //  line-by-line parsing
            abs_num = cur_directive.num;
            cur_directive.num = rel_num;
            rel_num+=parse_dr(cur_directive, data_label_table, data);
        }
    }

    catch (std::runtime_error &e) {
        throw CE(".data", e.what(), abs_num);
    }
    catch (std::logic_error &e) {
        throw CE(".data", e.what(), abs_num);
    }

    return data;
}

//  LINE PARSING

size_t parse_dr(NumberedLine data_str, std::unordered_set<ID> &data_label_table, std::unique_ptr<Data> &data) {
    std::istringstream tok_stream(data_str.line);
    my_std::Vec<std::string> tokens;
    std::string cur_token;
    size_t DataSize = 0;

    ///  TOKENIZATION  ///
    
    while (tok_stream >> cur_token) tokens.push_back(cur_token);
    if (tokens.size() != 2 && tokens.size() != 3) throw std::logic_error ("bad directive");

    //   PARSE LABEL

    ID label = "";

    label.addr = data_str.num;
    if (tokens[0].back() == ':') { 
        if (tokens[0][0] == ':') throw std::logic_error ("empty label");
        label = tokens[0].substr(0, tokens[0].length() - 1).c_str();  //  shrink trailing ':'
        if (tokens[1] == ".ascii") label.is_ascii = 1;
    } else {
        throw std::logic_error ("bad label");
    }

    if (tokens.size() != 3) {
        data->push_back(std::stoi(tokens[1]));
        return 1;
    }

    if (tokens[1] != ".ascii") throw std::logic_error ("bad ascii directive");

    for (auto sym : tokens[2]) {
        DataSize++;
        data->push_back(sym);
    }
    data->push_back(0);
    DataSize++;
    label.end_of_mem = label.addr + DataSize - 1;
    if (!data_label_table.insert(label).second)
        throw std::logic_error ("can't add label");
    return DataSize;
}

std::unique_ptr<Command> parse_cmd(const InstrSet &iset, NumberedLine cmd_str, 
const std::unordered_set<ID> &data_label_table, std::unordered_set<ID> &code_label_table) {

    std::istringstream tok_stream(cmd_str.line);
    my_std::Vec<std::string> tokens;
    std::string cur_token;

    ID label = "";

    ///  TOKENIZATION  ///
    
    while (tok_stream >> cur_token) tokens.push_back(cur_token);
    std::size_t num_tok = tokens.size();

    ///  Ucmd has max 3 tokens, Bcmd - 4, Tcmd - 5.
    if (num_tok > 5) throw std::logic_error("Too many tokens");
    if (num_tok < 2) throw std::logic_error("Incomplete command");

    ///  PARSING  ///

    std::size_t cur_tok_num = 0;

    //   PARSE LABEL

    label.addr = cmd_str.num;
    if (tokens[0].back() == ':') { 
        if (tokens[0][0] == ':') throw std::logic_error ("empty label");
        cur_tok_num++; label = tokens[0].substr(0, tokens[0].length() - 1).c_str();  //  shrink trailing ':'
        code_label_table.insert(label);
    } 

    //   PARSE SIGNIFICANT PART

    std::size_t num_sign_tok = num_tok-cur_tok_num;
    if (num_sign_tok > 4) throw std::logic_error("Too many significant tokens");

    //  1st significant token is operator  

    const Operator &oper = iset.FindOper(tokens[cur_tok_num++]); 

    //  parse 1st opd

    my_std::Vec<std::unique_ptr<Operand>> parsed_opds;
    std::unique_ptr<Operand> opd1;

    if (tokens[cur_tok_num][0] == '%')  {  //  register
        std::string reg_num (tokens[cur_tok_num].substr(2));
        opd1 = std::make_unique<GPRegister>(GPRegister(std::stoi(reg_num)));

    } else if (tokens[cur_tok_num][0] == '$')  {  //  data label
        ID data_label = FindLabel(data_label_table, tokens[cur_tok_num].substr(1).c_str());
        opd1 = std::make_unique<DataCell>(DataCell(data_label.addr));
    }  else if (!std::isdigit(tokens[cur_tok_num][0])) {  //  code label
        ID code_label = FindLabel(code_label_table, tokens[cur_tok_num].c_str());
        opd1 = std::make_unique<PCRegister>(PCRegister(code_label.addr));
    } else {
        throw std::logic_error("First operand can't be immediate");
    }

    parsed_opds.push_back(std::move(opd1));

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        std::unique_ptr<Command> ucmd = 
        std::make_unique<Command>(Command(label, oper, std::move(parsed_opds)));
        
        return ucmd;
    }

    //  parse 2nd opd

    std::unique_ptr<Operand> opd2;

    if (tokens[cur_tok_num][0] == '%')  {  //  register
        std::string reg_num (tokens[cur_tok_num].substr(2));
        opd2 = std::make_unique<GPRegister>(std::stoi(reg_num));
    } else if (tokens[cur_tok_num][0] == '$')  {  //  data label
        ID data_label = FindLabel(data_label_table, tokens[cur_tok_num].substr(1).c_str());
        if (data_label.is_ascii)
            opd2 = std::make_unique<String>(String(data_label.addr, data_label.end_of_mem));
        else 
            opd2 = std::make_unique<DataCell>(DataCell(data_label.addr));
    } else if (std::isdigit(tokens[cur_tok_num][0])) {  //  imm int
        opd2 = std::make_unique<ImmOperand>(std::stoi(tokens[cur_tok_num]));
    } else if (std::isalpha(tokens[cur_tok_num][0])) {  //  imm char
        opd2 = std::make_unique<ImmOperand>(static_cast<int>(tokens[cur_tok_num][0]));
    } else {
        throw std::logic_error("invalid 2nd operand");
    }

    parsed_opds.push_back(std::move(opd2));

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        std::unique_ptr<Command> bincmd = 
        std::make_unique<Command>(Command(label, oper, std::move(parsed_opds)));
        return bincmd;
    }

    //  parse 3rd opd

    std::unique_ptr<Operand> opd3;

    if (tokens[cur_tok_num][0] == '$')  {  //  data label
        ID data_label = FindLabel(data_label_table, tokens[cur_tok_num].substr(1).c_str());
        opd3 = std::make_unique<String>(String(data_label.addr, data_label.end_of_mem));
    }

    parsed_opds.push_back(std::move(opd3));

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        std::unique_ptr<Command> terncmd = 
        std::make_unique<Command>(Command(label, oper, std::move(parsed_opds)));
        return terncmd;
    }

    throw std::logic_error("wrong command");
} 

//  OTHER

std::logic_error CE (const char *section, const char *error, std::size_t line_num) {
    std::stringstream ss;
    ss << "in section " << section << ": CE on line " << line_num << ":\n" << error;
    return std::logic_error(ss.str());
}

const ID& FindLabel (const std::unordered_set<ID> &label_table, const ID &_id) {
    auto label = label_table.find(_id);
    if (label == label_table.end())
        throw std::logic_error("Unknown label: " + _id.id);
    else 
        return label._M_cur->_M_v();
}


