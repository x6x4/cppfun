
#include "prog.h"
#include <iostream>
#include <string>


void check_id (std::string name) {
    if (name.size() > 8)
        throw std::logic_error ("ID can't be more than 8 symbols\n");
    
    for (char c : name) {
        if (!std::isalpha(c))
            throw std::logic_error ("ID can only contain latin letters\n");
    }
}


std::ostream &operator<<(std::ostream &os, Command &cmd) {
    if (cmd.label.get_id().length()) os << cmd.label.get_id() << ": ";
    if (cmd.oper.get_name().length()) os << cmd.oper.get_name() << " ";
    if (cmd.op1.get_name().length()) os << cmd.op1.get_name() << " ";
    else os << cmd.op1.get_val() << " ";
    os << cmd.op2.get_name();
    return os; 
}

std::ostream &operator<<(std::ostream &os, ProgramMemory &pm) {
    for (auto& cmd : pm.prog)
        os << cmd << std::endl;

    return os;
}  



void load_program (const char *filename, ProgramMemory &pm) {

    std::ifstream is (filename);
    std::string command;
    
    if (is.is_open()) {

        while (std::getline(is, command)) {
            if (!command.compare("")) continue;
            if (!command.compare(".data")) break;
            pm.prog.push_back(make_cmd(command));
        }
        while (std::getline(is, command)) {
            pm.prog.push_back(make_data(command));
        }
    }

}

Command make_cmd (std::string command) {
    
    Command cmd;

    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;

    while (iss >> token) {
        tokens.push_back(token);
    }

    std::size_t cur_pos = 0;

    if (tokens[0].back() == ':') {
        
        //  shrink trailing ':'
        cmd.label = tokens[0].substr(0, tokens[0].length() - 1);

        //  operator is 2nd token    
        cmd.oper = tokens[1];

        cur_pos = 2;
    }

    //  operator is 1st token  
    else { cmd.oper = tokens[0]; cur_pos = 1; }

    if (tokens[cur_pos][0] == 'r' || tokens[cur_pos-1][0] == 'j') {
        //  operand 1 is 3rd/2nd token 
        cmd.op1 = tokens[cur_pos++];
    }
    else throw std::runtime_error ("CE: first operand must be a register\n");


    if (cur_pos != tokens.size()) {
        //  operand 2 is 4th/3rd token 
        cmd.op2 = tokens[cur_pos];
    }

    //std::cout << cmd << std::endl;

    return cmd;
}

Command make_data (std::string command) {
    
    Command cmd;

    std::istringstream iss(command);
    std::vector<std::string> tokens;
    std::string token;

    iss >> cmd.label >> cmd.op1;
    cmd.label = cmd.label.get_id().substr(0, cmd.label.get_id().length() - 1);
    return cmd;
}