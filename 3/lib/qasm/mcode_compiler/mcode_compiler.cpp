
#include "mcode_compiler.h"
#include "../../cpu/cpu.h"
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

MCode parser(InstrSet &iset, std::ifstream &is);
Command* parse_cmd(InstrSet &iset, std::string &cmd, std::size_t line_num);
std::logic_error CE (const char *error, std::size_t line_num);


std::ostream &operator<<(std::ostream &os, const MCode &mc) {
    mc.print_mcode(os);
    return os;
}


//  PARSING

MCode file_to_mcode (InstrSet &iset, const char *filename) {

    try {
        std::ifstream is (filename);
        if (is.is_open()) 
            return parser(iset, is);
        else 
            throw std::logic_error ("Wrong file");
    }

    catch (std::runtime_error &e) {
        throw e;
    }
    catch (std::logic_error &e) {
        throw e;
    }
} 

MCode parser(InstrSet &iset, std::ifstream &is) {

    ///  .TEXT SECTION  ///

    std::size_t line_num = 0;
    Command* cur_cmd;

    try {

        std::string command;

        MCode prog = MCode();

        while (std::getline(is, command)) {  //  line-by-line parsing
            line_num++;
            
            if (!command.compare("")) continue;    //  empty line
            if (command[0] == '#') continue;       //  comment
            if (!command.compare(".data")) break;  //  end of .text section
            
            cur_cmd = parse_cmd(iset, command, line_num);
            prog.add_cmd(cur_cmd);
        }

        return prog;
    }

    catch (std::runtime_error &e) {
        throw CE(e.what(), line_num);
    }
    catch (std::logic_error &e) {
        throw CE(e.what(), line_num);
    }
}


Command* parse_cmd(InstrSet &iset, std::string &cmd_str, std::size_t line_num) {

    std::istringstream tok_stream(cmd_str);
    std::vector<std::string> tokens;
    std::string cur_token;

    ID label = "";

    ///  TOKENIZATION  ///
    
    while (tok_stream >> cur_token) tokens.push_back(cur_token);
    std::size_t num_tok = tokens.size();

    ///  Ucmd has max 3 tokens, Bcmd - 4.
    if (num_tok > 4) throw std::logic_error("Too many tokens");


    ///  PARSING  ///

    std::size_t cur_tok_num = 0;

    //   PARSE LABEL

    if (tokens[0].back() == ':') {  
        if (num_tok == 1) {
            throw std::logic_error("empty command");
        }
        //label = tokens[0].substr(0, tokens[0].length() - 1);  //  shrink trailing ':'
        label.set_addr(line_num);

        cur_tok_num++;
    }

    //   PARSE SIGNIFICANT PART

    std::size_t num_sign_tok = num_tok-cur_tok_num;
    if (num_sign_tok > 3) throw std::logic_error("Too many significant tokens");

    if (num_sign_tok == 1) throw std::logic_error("expected operand");

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

    Operand* opd1;

    if (tokens[cur_tok_num][0] == 'r')  {  //  register
        std::string reg_num (tokens[cur_tok_num].substr(1));
        opd1 = new Register(std::stoi(reg_num));

    } else {
        throw std::logic_error("invalid 1st operand");
    }

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        UnaryCommand* ucmd = new UnaryCommand(label, uoper, opd1);
        return ucmd;
    }

    //  parse 2nd opd

    Operand* opd2;

    if (tokens[cur_tok_num][0] == 'r')  {  //  register
        std::string reg_num (tokens[cur_tok_num].substr(1));
        opd2 = new Register(std::stoi(reg_num));

    } else {
        throw std::logic_error("invalid 2nd operand");
    }

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        BinaryCommand* bincmd = new BinaryCommand(label, binoper, opd1, opd2);
        return bincmd;
    }

    throw std::logic_error("Wrong command");

} 


std::logic_error CE (const char *error, std::size_t line_num) {
    std::stringstream ss;
    ss << "CE on line " << line_num << ": " << error;
    return std::logic_error(ss.str());
}




