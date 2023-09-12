
#include <cstddef>
#include <string>
#include <vector>


class Res {
public:
    std::string name;
    std::size_t cons;
    std::size_t prod;
    std::size_t price;

    //  constructors

    

    //  union same resources

    //  compare two resources by name
    bool operator<(const Res& r) const { return name < r.name; }

    //  calculate resource week profit

    //  multiply resource turnover 
};

template<typename T> 
class Vector {
public:
    T* data;
    std::size_t cap;
    std::size_t sz; 

    std::size_t begin () { return 0; }
    std::size_t end ()   { return this->sz - 1; }
    
};


class Res_Table {
public:
    Vector<Res> vec;

    //  constructors

    //  get resource by name
    Res &operator[](std::string name);

    //  set resource name

    enum state {
        EMPTY,
        PART,
        FULL
    };

    //  check table state
    int check () noexcept;

    //  delete resource
    void del (Res r);

    //  calculate total week profit for all resources

    //  multiply turnover for all resources
};

