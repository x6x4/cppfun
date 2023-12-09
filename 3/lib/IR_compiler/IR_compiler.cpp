
#include "fwd_IR_compiler.h"
#include <cstddef>
#include <cstdlib>
#include <fmt/core.h>
#include <sstream>
#include <stdexcept>
#include <string>
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
    
    std::ifstream iss(filename);
    if (iss.fail()) throw std::logic_error ("Wrong file");
    strings program = to_strings(iss);

    cpu.load_mem(file_to_mcode(cpu.iSet(), program, avl_bps));
    return program;
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
        throw e;
    }
    catch (std::logic_error &e) {
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

std::unique_ptr<Data> parse_data (strings vec_data, std::unordered_set<ID> &data_label_table) {

    std::unique_ptr<Data> data = std::make_unique<Data>(Data());    
    NumberedLine directive;
    std::size_t num = 0;

    try {
        for (auto _directive : vec_data) {  //  line-by-line parsing
            directive.num = _directive.num;
            _directive.num = num++;
            int data_cell = parse_dr(_directive, data_label_table);
            data->push_back(data_cell);
        }
    }

    catch (std::runtime_error &e) {
        throw CE(".data", e.what(), directive.num);
    }
    catch (std::logic_error &e) {
        throw CE(".data", e.what(), directive.num);
    }

    return data;
}

std::unique_ptr<SafeText> parse_text(const InstrSet &iset, strings &vec_text, const std::unordered_set<ID> &data_label_table) {

    std::unique_ptr<Command> cur_cmd;
    std::unordered_set<ID> code_label_table;
    std::size_t cur_num = 0;

    std::unique_ptr<SafeText> text = std::make_unique<SafeText>(SafeText());
 
    try {
        for (auto cmd : vec_text) {  //  line-by-line parsing
            cur_num = cmd.num;
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

//  LINE PARSING

int parse_dr(NumberedLine data_str, std::unordered_set<ID> &data_label_table) {
    std::istringstream tok_stream(data_str.line);
    my_std::Vec<std::string> tokens;
    std::string cur_token;

    ///  TOKENIZATION  ///
    
    while (tok_stream >> cur_token) tokens.push_back(cur_token);
    if (tokens.size() != 2) throw std::logic_error ("bad directive");

    //   PARSE LABEL

    ID label = "";

    label.set_addr(data_str.num);
    if (tokens[0].back() == ':') { 
        if (tokens[0][0] == ':') throw std::logic_error ("empty label");
        label = tokens[0].substr(0, tokens[0].length() - 1).c_str();  //  shrink trailing ':'
        if (!data_label_table.insert(label).second)
            throw std::logic_error ("can't add label");
    } else {
        throw std::logic_error ("bad label");
    }

    return std::stoi(tokens[1]);
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

    ///  Ucmd has max 3 tokens, Bcmd - 4.
    if (num_tok > 4) throw std::logic_error("Too many tokens");
    if (num_tok < 2) throw std::logic_error("Incomplete command");

    ///  PARSING  ///

    std::size_t cur_tok_num = 0;

    //   PARSE LABEL

    label.set_addr(cmd_str.num);
    if (tokens[0].back() == ':') { 
        if (tokens[0][0] == ':') throw std::logic_error ("empty label");
        cur_tok_num++; label = tokens[0].substr(0, tokens[0].length() - 1).c_str();  //  shrink trailing ':'
        code_label_table.insert(label);
    } 

    //   PARSE SIGNIFICANT PART

    std::size_t num_sign_tok = num_tok-cur_tok_num;
    if (num_sign_tok > 3) throw std::logic_error("Too many significant tokens");

    //  1st significant token is operator  

    UnaryOperator uoper;
    BinaryOperator binoper;

    if (num_sign_tok == 2) {         //  parse Uoper

        uoper = iset.FindUnOper(tokens[cur_tok_num]); 

    } else if (num_sign_tok == 3) {  //  parse Boper

        binoper = iset.FindBinOper(tokens[cur_tok_num]); 
    }

    cur_tok_num++;

    //  parse 1st opd

    std::unique_ptr<Operand> opd1;

    if (tokens[cur_tok_num][0] == '%')  {  //  register
        std::string reg_num (tokens[cur_tok_num].substr(2));
        opd1 = std::make_unique<GPRegister>(GPRegister(std::stoi(reg_num)));

    } else if (tokens[cur_tok_num][0] == '$')  {  //  data label
        ID data_label = FindLabel(data_label_table, tokens[cur_tok_num].substr(1).c_str());
        opd1 = std::make_unique<DataCell>(DataCell(data_label.get_addr()));
    }  else if (!std::isdigit(tokens[cur_tok_num][0])) {  //  code label
        ID code_label = FindLabel(code_label_table, tokens[cur_tok_num].c_str());
        opd1 = std::make_unique<PCRegister>(PCRegister(code_label.get_addr()));
    } else {
        throw std::logic_error("First operand can't be immediate");
    }

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        std::unique_ptr<UnaryCommand> ucmd = 
        std::make_unique<UnaryCommand>(UnaryCommand(label, uoper, std::move(opd1)));
        
        return ucmd;
    }

    //  parse 2nd opd


    std::unique_ptr<Operand> opd2;

    if (tokens[cur_tok_num][0] == '%')  {  //  register
        std::string reg_num (tokens[cur_tok_num].substr(2));
        opd2 = std::make_unique<GPRegister>(std::stoi(reg_num));
    } else if (tokens[cur_tok_num][0] == '$')  {  //  data label
        ID data_label = FindLabel(data_label_table, tokens[cur_tok_num].substr(1).c_str());
        opd2 = std::make_unique<DataCell>(DataCell(data_label.get_addr()));
    } else if (std::isdigit(tokens[cur_tok_num][0])) {  //  imm
        opd2 = std::make_unique<Operand>(std::stoi(tokens[cur_tok_num]));
    } else {
        throw std::logic_error("invalid 2nd operand");
    }

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        std::unique_ptr<BinaryCommand> bincmd = 
        std::make_unique<BinaryCommand>(BinaryCommand(label, binoper, std::move(opd1), std::move(opd2)));
        return bincmd;
    }

    throw std::logic_error("wrong command");
} 

//  OTHER

std::logic_error CE (const char *section, const char *error, std::size_t line_num) {
    std::stringstream ss;
    ss << "in section " << section << ": CE on line " << line_num << ":\n" << error;
    return std::logic_error(ss.str());
}

const ID& FindLabel (const std::unordered_set<ID> &label_table, const ID &id) {
    auto label = label_table.find(id);
    if (label == label_table.end())
        throw std::logic_error("Unknown label: " + id.get_id());
    else 
        return label._M_cur->_M_v();
}


