#include <sstream>
#include "../cpu/cpu.h"
#include "mem.h"
#include <utility>


//  PARSING

using MPair = std::pair<MCode<Directive>, MCode<Command>>;

MPair parser(InstrSet &iset, std::ifstream &is);
Command* parse_cmd(InstrSet &iset, std::string &cmd, std::size_t line_num, 
                    std::unordered_set<ID> &data_label_table, std::unordered_set<ID> &code_label_table);
Directive* parse_dr(std::string &dr, std::size_t line_num, std::unordered_set<ID> &data_label_table);
std::logic_error CE (const char *error, std::size_t line_num);


MPair file_to_mcode (InstrSet &iset, const char *filename) {

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


MPair parser(InstrSet &iset, std::ifstream &is) {


    std::size_t line_num = 0;
    Command* cur_cmd;
    Directive* cur_dr;

    std::unordered_set<ID> code_label_table;
    std::unordered_set<ID> data_label_table;

    try {

        ///  .DATA SECTION  ///

        std::string directive;

        MCode data = MCode<Directive>();

        while (std::getline(is, directive)) {  //  line-by-line parsing
            
            if (!directive.compare("")) continue;    //  empty line
            if (directive[0] == '#') continue;       //  comment
            if (!directive.compare(".text")) break;  //  end of .data section

            cur_dr = parse_dr(directive, line_num, data_label_table);
            line_num++;
            data.add(cur_dr);
        }

        ///  .TEXT SECTION  ///

        std::string command;

        MCode prog = MCode<Command>();

        while (std::getline(is, command)) {  //  line-by-line parsing
            
            if (!command.compare("")) continue;    //  empty line
            if (command[0] == '#') continue;       //  comment
            
            cur_cmd = parse_cmd(iset, command, line_num, data_label_table, code_label_table);
            line_num++;
            prog.add(cur_cmd);
        }


        return std::make_pair(data, prog);
    }

    catch (std::runtime_error &e) {
        throw CE(e.what(), line_num);
    }
    catch (std::logic_error &e) {
        throw CE(e.what(), line_num);
    }
}

const ID& FindLabel (std::unordered_set<ID> &label_table, const ID &id) {
    auto label = label_table.find(id);
    if (label == label_table.end())
        throw std::logic_error("Unknown code label");
    else 
        return label._M_cur->_M_v();
}

Command* parse_cmd(InstrSet &iset, std::string &cmd_str, std::size_t line_num, 
std::unordered_set<ID> &data_label_table, std::unordered_set<ID> &code_label_table) {

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
        opd1 = std::move(std::make_unique<GPRegister>(GPRegister(std::stoi(reg_num))));

    } else if (tokens[cur_tok_num][0] == '$')  {  //  data label
        ID data_label = FindLabel(data_label_table, tokens[cur_tok_num].substr(1));
        opd1 = std::move(std::make_unique<DataCell>(DataCell(data_label.get_addr())));
    }
    else {
        ID code_label = FindLabel(code_label_table, tokens[cur_tok_num]);
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

    } else if (tokens[cur_tok_num][0] == '$')  {  //  data label
        ID data_label = FindLabel(data_label_table, tokens[cur_tok_num].substr(1));
        opd2 = std::move(std::make_unique<DataCell>(DataCell(data_label.get_addr())));
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




