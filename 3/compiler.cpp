

void preproc(std::ifstream &is);

void parser(std::ifstream &is) {

    ///  .DATA SECTION  ///

    std::string directive;

    while (std::getline(is, directive)) {
        make_data(directive, m);
    }
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