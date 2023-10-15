
#include "mcode_compiler.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>


std::ostream &operator<<(std::ostream &os, const MCode &mc) {
    print_mprog(os, mc.prog);
    return os;
}

MCode &to_mcode (InstrSet &iset, const char *filename) {

    try {
        std::ifstream is (filename);
        if (is.is_open())
            return compile (iset, is);
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

MCode &compile (InstrSet &iset, std::ifstream &is) {

    try {
        mprog prog = parser(iset, is);

        MCode *code = new MCode(prog);
        return *code;
    }
        
    catch (std::runtime_error &e) {
        throw e;
    }
    catch (std::logic_error &e) {
        throw e;
    }
}

Command &parse_cmd(InstrSet &iset, std::string &cmd, std::size_t line_num);

std::logic_error CE (const char *error, std::size_t line_num);

mprog &parser(InstrSet &iset, std::ifstream &is) {

    ///  .TEXT SECTION  ///

    std::size_t line_num = 0;

    try {

        std::string command;

        mprog *prog = new mprog;

        while (std::getline(is, command)) {
            line_num++;
            
            if (!command.compare("")) continue;    //  empty line
            if (command[0] == '#') continue;       //  comment
            if (!command.compare(".data")) break;  //  end of .text section
            
            Command &cur_cmd = parse_cmd(iset, command, line_num);
            prog->push_back(&cur_cmd);
        }

        return *prog;
    }

    catch (std::runtime_error &e) {
        throw CE(e.what(), line_num);
    }
    catch (std::logic_error &e) {
        throw CE(e.what(), line_num);
    }
}



Command &parse_cmd(InstrSet &iset, std::string &cmd_str, std::size_t line_num) {

    std::istringstream iss(cmd_str);
    std::vector<std::string> tokens;
    std::string token;
    std::string label = "";


    ///  TOKENIZATION  ///
    ///  Ucmd has 2 significant tokens, Bcmd - 3.

    while (iss >> token) {
        tokens.push_back(token);
    }

    std::size_t cur_tok_num = 0;

    ///  PARSE LABEL  ///

    if (tokens[0].back() == ':') {  
        if (tokens.size() == 1) {
            throw CE("empty command", line_num);
        }
        label = tokens[0].substr(0, tokens[0].length() - 1);  //  shrink trailing ':'

        cur_tok_num++;
    }

    ///  PARSE SIGNIFICANT PART  ///

    std::size_t num_sign_tok = tokens.size()-cur_tok_num;

    if (num_sign_tok == 1) throw CE("expected operand", line_num);

    if (num_sign_tok == 2) {  //  parse Ucmd 
        
        UnaryOperator &uoper = iset.FindUnOper(tokens[cur_tok_num]);  //  2nd token is operator  
        cur_tok_num++;

        if (tokens[cur_tok_num][0] == 'r')  {  //  register
            std::string reg_num (tokens[cur_tok_num].substr(1));
            Operand &opd1 = *(new Register(std::stoi(reg_num)));

            UnaryCommand *ucmd = new UnaryCommand(label, uoper, opd1);
            return *ucmd;
        } else {
            throw CE("invalid operand", line_num);
        }

    } else if (num_sign_tok == 3) {  //  parse Bcmd

        BinaryOperator boper = iset.FindBinOper(tokens[1]);  //  2nd token is operator  
        cur_tok_num++;
        throw CE("binary command", line_num);

    } else {
        throw CE("invalid number of tokens", line_num);
    }

    ///  PARSE OPERAND1  ///

/*    if (tokens[cur_pos][0] == 'r') {  //  register
        cmd_str.op1 = tokens[cur_pos];
    } 
    else if (!std::isdigit(tokens[cur_pos][0]) //  1st operand can't be imm
        && cur_pos == tokens.size()-1) {  //  code label is always 1st and last operand
        cmd_str.op1 = tokens[cur_pos];
    }
    else throw CE("first operand must be a register");
    
    cur_pos++;*/

    ///  PARSE OPERAND2  ///

    /*if (cur_pos != tokens.size()) {
        if (tokens[cur_pos][0] == 'r'  //  register
            || std::isdigit(tokens[cur_pos][0])) {  // imm operand
            cmd_str.op2 = tokens[cur_pos];
        } 
        else {  //  data label is always 2nd operand
            l.has_data.push_back({l.cur_cmd, tokens[cur_pos]}); 
        }
    }*/

    //std::cout << cmd << std::endl;
}

std::logic_error CE (const char *error, std::size_t line_num) {
    std::stringstream ss;
    ss << "CE on line " << line_num << ": " << error;
    return std::logic_error(ss.str());
}

void f() {
    int a = 0xABCD;
    int addr = 0xFFFF;
    int mov = 0x5555;
    std::vector<int> cmd = {mov, a, addr};
}



/*      mov r5 5
        inc r8
        jmp loop
code1:  mov r6 data1
        neg r3
        cmp r5 r8
        jne loop*/

    /*try {

        auto r = Register(5);
        auto dl = DataLabel(0x10);

        BinaryCommand mov1 {Op_Mov(), r, dl};
        //std::cout << dl << std::endl;

        UnaryCommand inc {Op_Inc(), Register(8)};

        UnaryCommand jmp {Op_Jmp(), DataLabel(0x2)};

        //BinaryCommand mov {"code1", Op_Mov(), Register(6), "data1"};

        UnaryCommand neg {Op_Neg(), DataLabel(0x0)};

        BinaryCommand cmp {Op_Cmp(), Register(5), Register(8)};
 
        UnaryCommand jne (Op_Jne(), DataLabel(0x2));*/

        //, &inc, &jmp, &neg, &cmp, &jne};
        //m.pm.prog = prog_t{&mov1};
        //mov1.Command::print(std::cout);
        //std::cout << typeid(m.dm).name() << std::endl;

        //std::cout << m.pm;
    //    compile (src, m);
