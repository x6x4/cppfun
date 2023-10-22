
#include "mcode_compiler.h"
#include "../../cpu/cpu.h"


//  MCode

MCode::~MCode () {
    for (auto cmd : cmds) { delete cmd; cmd = nullptr; }
    cmds.clear();
}

MCode &MCode::operator= (const MCode& _mc) {
    this->cmds = _mc.cmds;
    for (std::size_t i = 0; i < _mc.cmds.size(); i++) 
        this->cmds[i] = _mc.cmds[i]->clone();

    return *this;
}

std::size_t MCode::num_lines () { return cmds.size(); }

Command *MCode::operator[] (std::size_t num) const { 
    if (num < cmds.size()) return cmds[num]; 
    throw std::logic_error("Access out of bounds");
}

void MCode::add_cmd (Command* cmd) { cmds.push_back(cmd); };

void MCode::print_mcode (std::ostream &os) const {
    for (auto cmd : cmds) os << *cmd << '\n';
}

std::ostream &operator<<(std::ostream &os, const MCode &mc) {
    mc.print_mcode(os);
    return os;
}


//  PARSING

MCode parser(InstrSet &iset, std::ifstream &is);
Command* parse_cmd(InstrSet &iset, std::string &cmd, std::size_t line_num, std::unordered_set<ID> &label_table);
std::logic_error CE (const char *error, std::size_t line_num);


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

    std::unordered_set<ID> label_table;

    try {

        std::string command;

        MCode prog = MCode();

        while (std::getline(is, command)) {  //  line-by-line parsing
            
            if (!command.compare("")) continue;    //  empty line
            if (command[0] == '#') continue;       //  comment
            if (!command.compare(".data")) break;  //  end of .text section
            
            cur_cmd = parse_cmd(iset, command, line_num, label_table);
            line_num++;
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

const ID& FindCodeLabel (std::unordered_set<ID> &label_table, const ID &id) {
    auto code_label = label_table.find(id);
    if (code_label == label_table.end())
        throw std::logic_error("Unknown code label");
    else 
        return code_label._M_cur->_M_v();
}

Command* parse_cmd(InstrSet &iset, std::string &cmd_str, std::size_t line_num, std::unordered_set<ID> &label_table) {

    std::istringstream tok_stream(cmd_str);
    std::vector<std::string> tokens;
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

    label.set_addr(line_num);
    if (tokens[0].back() == ':') { 
        cur_tok_num++; label = tokens[0].substr(0, tokens[0].length() - 1);  //  shrink trailing ':'
        label_table.insert(label);
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
        opd1 = std::move(std::make_unique<GPRegister>(GPRegister(std::stoi(reg_num))));

    } else {
        ID code_label = FindCodeLabel(label_table, tokens[cur_tok_num]);
        opd1 = std::move(std::make_unique<SPRegister>(SPRegister(code_label.get_addr())));
    }

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        UnaryCommand* ucmd = new UnaryCommand(label, uoper, std::move(opd1));
        return ucmd;
    }

    //  parse 2nd opd

    std::unique_ptr<Operand> opd2;

    if (tokens[cur_tok_num][0] == '%')  {  //  register
        std::string reg_num (tokens[cur_tok_num].substr(2));
        opd2 = std::move(std::make_unique<GPRegister>(std::stoi(reg_num)));

    } else {
        throw std::logic_error("invalid 2nd operand");
    }

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        BinaryCommand* bincmd = new BinaryCommand(label, binoper, std::move(opd1), std::move(opd2));
        return bincmd;
    }

    throw std::logic_error("Wrong command");

} 


std::logic_error CE (const char *error, std::size_t line_num) {
    std::stringstream ss;
    ss << "CE on line " << line_num << ": " << error;
    return std::logic_error(ss.str());
}




