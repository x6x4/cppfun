
#include "fwd_IR_compiler.h"
#include <sstream>


// MAIN

void load_text_cpu (CPU &cpu, const char *program_text) {
    std::istringstream iss(program_text);
    cpu.load_mem(file_to_mcode(cpu.iSet(), iss));
}

void load_file_cpu (CPU &cpu, const char *filename) {
    cpu.load_mem(file_to_mcode(cpu.iSet(), filename));
}

Mem file_to_mcode (const InstrSet &iset, const char *filename) {
    std::ifstream is (filename);
    return file_to_mcode(iset, is);
} 

Mem file_to_mcode (const InstrSet &iset, std::istream &is) {

    std::unordered_set<ID> data_label_table;

    try {
        //if (is.is_open()) {
            strings vec = preproc_text(is);
            std::unique_ptr<Data> data = parse_data(is, data_label_table);
            std::unique_ptr<SafeText> text = parse_text(iset, vec, data_label_table);

            return std::make_pair(std::move(data), std::move(text));
        //}
        //else 
            throw std::logic_error ("Wrong file");
    }

    catch (std::runtime_error &e) {
        throw e;
    }
    catch (std::logic_error &e) {
        throw e;
    }
}

//  PREPROC AND SECTIONS PARSING

strings preproc_text (std::istream &is) {
    
    strings vec;
    std::string line;
        
    while (std::getline(is, line)) {
        if (!line.compare(".text")) continue;    //  start of .text section
        if (!line.compare("")) continue;    //  empty line
        if (line[0] == '#') continue;       //  comment
        if (!line.compare(".data")) break;  //  .data section

        vec.push_back(line);
    }

    return vec;
}

std::unique_ptr<Data> parse_data (std::istream &is, std::unordered_set<ID> &data_label_table) {

    std::unique_ptr<Data> data = std::make_unique<Data>(Data());
    std::size_t line_num = 0;
    std::size_t user_line_num = 0;

    try {
        std::string directive;

        while (std::getline(is, directive)) {  //  line-by-line parsing

            user_line_num++;
  
            if (!directive.compare("")) continue;    //  empty line
            if (directive[0] == '#') continue;       //  comment

            int data_cell = parse_dr(directive, line_num, data_label_table);
            data->push_back(data_cell);
            line_num++;
        }
    }

    catch (std::runtime_error &e) {
        throw CE(".data", e.what(), user_line_num);
    }
    catch (std::logic_error &e) {
        throw CE(".data", e.what(), user_line_num);
    }

    return data;
}

std::unique_ptr<SafeText> parse_text(const InstrSet &iset, strings &vec, const std::unordered_set<ID> &data_label_table) {

    std::size_t line_num = 0;
    std::size_t user_line_num = 0;
    std::unique_ptr<Command> cur_cmd;
    std::unordered_set<ID> code_label_table;

    std::unique_ptr<SafeText> text = std::make_unique<SafeText>(SafeText());
 
    try {
        for (auto cmd : vec) {  //  line-by-line parsing
            user_line_num++;
            
            cur_cmd = parse_cmd(iset, cmd, line_num, data_label_table, code_label_table);
            line_num++;
            
            text->push_back(std::move(cur_cmd));
        }
    }

    catch (std::runtime_error &e) {
        throw CE(".text", e.what(), user_line_num);
    }
    catch (std::logic_error &e) {
        throw CE(".text", e.what(), user_line_num);
    }

    return text;
}

//  LINE PARSING

int parse_dr(std::string &data_str, std::size_t line_num, std::unordered_set<ID> &data_label_table) {
    std::istringstream tok_stream(data_str);
    std::vector<std::string> tokens;
    std::string cur_token;

    ///  TOKENIZATION  ///
    
    while (tok_stream >> cur_token) tokens.push_back(cur_token);
    if (tokens.size() != 2) throw std::logic_error ("bad directive");

    //   PARSE LABEL

    ID label = "";

    label.set_addr(line_num);
    if (tokens[0].back() == ':') { 
        if (tokens[0][0] == ':') throw std::logic_error ("empty label");
        label = tokens[0].substr(0, tokens[0].length() - 1).c_str();  //  shrink trailing ':'
        data_label_table.insert(label);
    } else {
        throw std::logic_error ("bad label");
    }

    return std::stoi(tokens[1]);
}

std::unique_ptr<Command> parse_cmd(const InstrSet &iset, std::string &cmd_str, std::size_t line_num, 
const std::unordered_set<ID> &data_label_table, std::unordered_set<ID> &code_label_table) {

    std::istringstream tok_stream(cmd_str);
    strings tokens;
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
        opd1 = std::move(std::make_unique<GPRegister>(GPRegister(std::stoi(reg_num))));

    } else if (tokens[cur_tok_num][0] == '$')  {  //  data label
        ID data_label = FindLabel(data_label_table, tokens[cur_tok_num].substr(1).c_str());
        opd1 = std::move(std::make_unique<DataCell>(DataCell(data_label.get_addr())));
    }  else if (!std::isdigit(tokens[cur_tok_num][0])) {  //  code label
        ID code_label = FindLabel(code_label_table, tokens[cur_tok_num].c_str());
        opd1 = std::move(std::make_unique<SPRegister>(SPRegister(code_label.get_addr())));
    } else {
        throw std::logic_error("First operand can't be immediate");
    }

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        std::unique_ptr<UnaryCommand> ucmd = 
        std::make_unique<UnaryCommand>(std::move(UnaryCommand(label, uoper, std::move(opd1))));
        
        return ucmd;
    }

    //  parse 2nd opd

    std::unique_ptr<Operand> opd2;

    if (tokens[cur_tok_num][0] == '%')  {  //  register
        std::string reg_num (tokens[cur_tok_num].substr(2));
        opd2 = std::move(std::make_unique<GPRegister>(std::stoi(reg_num)));

    } else if (tokens[cur_tok_num][0] == '$')  {  //  data label
        ID data_label = FindLabel(data_label_table, tokens[cur_tok_num].substr(1).c_str());
        opd2 = std::move(std::make_unique<DataCell>(DataCell(data_label.get_addr())));
    } else if (std::isdigit(tokens[cur_tok_num][0])) {  //  imm
        opd2 = std::move(std::make_unique<Operand>(std::stoi(tokens[cur_tok_num])));
    } else {
        throw std::logic_error("invalid 2nd operand");
    }

    cur_tok_num++;

    if (cur_tok_num == num_tok) {
        std::unique_ptr<BinaryCommand> bincmd = 
        std::make_unique<BinaryCommand>(std::move(BinaryCommand(label, binoper, std::move(opd1), std::move(opd2))));
        return bincmd;
    }

    throw std::logic_error("wrong command");
} 

//  OTHER

std::logic_error CE (const char *section, const char *error, std::size_t line_num) {
    std::stringstream ss;
    ss << "In section " << section << ": CE on line " << line_num << ": " << error;
    return std::logic_error(ss.str());
}

const ID& FindLabel (const std::unordered_set<ID> &label_table, const ID &id) {
    auto label = label_table.find(id);
    if (label == label_table.end())
        throw std::logic_error("Unknown label: " + id.get_id());
    else 
        return label._M_cur->_M_v();
}


