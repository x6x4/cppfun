
#include "../mem/mem.h"
#include <cctype>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>


std::size_t line_num = 0;

std::runtime_error CE (const char *error) {
    std::stringstream ss;
    ss << "CE on line " << line_num << ": " << error << std::endl;
    return std::runtime_error(ss.str());
}


void check_id (std::string name) {
    if (name.size() > 8)
        throw std::logic_error ("ID can't be more than 8 symbols\n");
    
    for (char c : name) {
        if (!std::isalpha(c))
            throw std::logic_error ("ID can only contain latin letters\n");
    }
}


std::ostream &operator<<(std::ostream &os, Command &cmd) {

    return os;
}

std::ostream &operator<<(std::ostream &os, ProgramMemory &pm) {
    for (auto& cmd : pm.prog)
        os << cmd << std::endl;

    return os;
}  



void parser(std::ifstream &is, Memory &m, Data_labels &dl);
Command make_instr (std::string command, Data_labels &dl);
void make_data (std::string command, Memory &m);

void preproc(std::ifstream &is, Memory &m, Data_labels &dl);

void compile (const char *filename, Memory &m) {

    std::ifstream is (filename);
    
    Data_labels dl;

    if (is.is_open()) {

        parser(is, m, dl);
        preproc(is, m, dl);
    }
    else {
        throw std::runtime_error ("Wrong file\n");
    }

}

void parser(std::ifstream &is, Memory &m, Data_labels &l) {

    ///  .TEXT SECTION  ///

    std::string command;

    while (std::getline(is, command)) {
        line_num++;
        
        if (!command.compare("")) continue;    //  empty line
        if (command[0] == '#') continue;    //  comment
        if (!command.compare(".data")) break;  //  end of .text section
        m.pm.prog.push_back(make_instr(command, l));
    }

    ///  .DATA SECTION  ///

    std::string directive;

    while (std::getline(is, directive)) {
        make_data(directive, m);
    }
}

Command make_instr (std::string command, Data_labels &l) {

    l.cur_cmd++;
    
    Command cmd;

    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;


    ///  TOKENIZATION  ///

    while (iss >> token) {
        tokens.push_back(token);
    }

    std::size_t cur_pos = 0;

    ///  PARSE LABEL & OPER  ///

    if (tokens[0].back() == ':') {  
        if (tokens.size() == 1) {
            throw CE("empty label");
        }
        cmd.label = tokens[0].substr(0, tokens[0].length() - 1);  //  shrink trailing ':'
        
        if (tokens.size() == 2)
            throw CE("expected operator");
        cmd.oper = tokens[1];  //  2nd token is operator  
        cur_pos = 2;
    }

    ///  PARSE NO LABEL & OPER  ///

    else { 
        cmd.oper = tokens[0];  //  1st token is operator
        cur_pos = 1; 
    }

    if (tokens.size() == 1) {
        throw CE("missing operand");
    }

    ///  PARSE OPERAND1  ///

    if (tokens[cur_pos][0] == 'r') {  //  register
        cmd.op1 = tokens[cur_pos];
    } 
    else if (!std::isdigit(tokens[cur_pos][0]) //  1st operand can't be imm
        && cur_pos == tokens.size()-1) {  //  code label is always 1st and last operand
        cmd.op1 = tokens[cur_pos];
    }
    else throw CE("first operand must be a register");
    
    cur_pos++;

    ///  PARSE OPERAND2  ///

    if (cur_pos != tokens.size()) {
        if (tokens[cur_pos][0] == 'r'  //  register
            || std::isdigit(tokens[cur_pos][0])) {  // imm operand
            cmd.op2 = tokens[cur_pos];
        } 
        else {  //  data label is always 2nd operand
            l.has_data.push_back({l.cur_cmd, tokens[cur_pos]}); 
        }
    }

    //std::cout << cmd << std::endl;

    return cmd;
}

void make_data (std::string directive, Memory &m) {
    
    ID label; int val;
    std::istringstream iss(directive);

    iss >> label >> val;
    label = label.get_id().substr(0, label.get_id().length() - 1);

    m.dm += {label.get_id(), val};
}

void preproc(std::ifstream &is, Memory &m, Data_labels &l) {

    for (const auto& cmd : l.has_data) {
        
        auto pair = m.dm[cmd.second];
        //  data label is always 2nd operand
        if (pair.first) m.pm.prog[cmd.first].op2.set_addr(pair.second);
        else throw std::runtime_error ("Undefined label \"" + cmd.second + "\"\n");
    }
}