
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


/*void Register::print (std::ostream &os) const {
    os << num << " " << val();
}

void DataLabel::print (std::ostream &os) const {
    os << "0x" << static_cast<int>(label) << " " << val();
}*/