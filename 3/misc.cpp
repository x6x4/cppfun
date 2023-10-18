
/*      mov r5 5
        jmp loop
code1:  mov r6 data1
        neg r3
        cmp r5 r8
        jne loop*/


class Singleton {

    Singleton () {};

    static Singleton *ptr;

public:

    static Singleton &getInstance () {
        if (!ptr) ptr = new Singleton();
        return *ptr;
    }
};

Singleton *Singleton::ptr = nullptr;


void f() {
    int a = 0xABCD;
    int addr = 0xFFFF;
    int mov = 0x5555;
    std::vector<int> cmd = {mov, a, addr};
}

//  DERIVED FROM OPERAND

using addr_t = std::size_t;



class DataLabel : public Operand {

protected:
    void print (std::ostream &os) const override;

    addr_t label = 0;

public:
    DataLabel(addr_t _lbl) : label(_lbl) {};
    //DataLabel &Datalabel(DataLabel dtlbl) { *this = dtlbl; return *this; } 
    //DataLabel &Datalabel(DataLabel &&dtlbl) { *this = dtlbl; return *this; }
};


/*

void DataLabel::print (std::ostream &os) const {
    os << "0x" << static_cast<int>(label) << " " << val();
}*/

/* 
    else if (!std::isdigit(tokens[cur_pos][0]) //  1st operand can't be imm
        && cur_pos == tokens.size()-1) {  //  code label is always 1st and last operand
        cmd_str.op1 = tokens[cur_pos];
    }
    else throw CE("first operand must be a register");
    
    cur_pos++;

        if (tokens[cur_pos][0] == 'r'  //  register
            || std::isdigit(tokens[cur_pos][0])) {  // imm operand
            cmd_str.op2 = tokens[cur_pos];
        } 
        else {  //  data label is always 2nd operand
            l.has_data.push_back({l.cur_cmd, tokens[cur_pos]}); 
        }
    }*/