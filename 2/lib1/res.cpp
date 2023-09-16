#include "res.h"


//  CLASS Res

void Res::check_fields () {
    if (cons < 0)
        throw std::invalid_argument ("Negative consumption is not allowed");
    if (prod < 0)
        throw std::invalid_argument ("Negative production is not allowed");
    if (price < 0)
        throw std::invalid_argument ("Negative price is not allowed");
}

//  Constructors

Res::Res (std::string name, int cons, int prod, int price) : 
        name(name), cons(cons), prod(prod), price(price) {
    check_fields();
};

Res::Res (std::string name, int price) : name(name), price(price) {
    if (price < 0)
        throw std::invalid_argument ("Negative price is not allowed");        
};

//  Setters

Res &Res::set_name (std::string name) noexcept { this->name = name;   return *this;}

Res &Res::set_cons  (int cons)  { 
    if (cons < 0)
        throw std::invalid_argument ("Negative consumption is not allowed");
    this->cons = cons;   
    return *this;
}

Res &Res::set_prod  (int prod)  { 
    if (prod < 0)
        throw std::invalid_argument ("Negative production is not allowed");
    this->prod = prod;   
    return *this;
}

Res &Res::set_price  (int price)  { 
    if (price < 0)
        throw std::invalid_argument ("Negative price is not allowed");
    this->price = price;   
    return *this;
}

//  Other

Res operator+ (const Res &r1, const Res &r2) {

    if (r1.name != r2.name) {
        throw std::logic_error ("Can't join distinct resources.");
    }

    Res sum;

    sum.name = r1.name;
    sum.cons = r1.cons + r2.cons;
    sum.prod = r1.prod + r2.prod;
    sum.price = (r1.price < r2.price) ? r1.price : r2.price;

    return sum;
}

std::weak_ordering operator<=> (const Res& r1, const Res& r2) noexcept {
    if (r1.name < r2.name)
        return std::weak_ordering::less;
    else if (r1.name > r2.name)
        return std::weak_ordering::greater;
    else 
        return std::weak_ordering::equivalent;
};

int Res::get_profit () noexcept {
    return (this->prod - this->cons)*this->price*7;
}

Res Res::operator* (int n) noexcept {
    this->cons *= n;
    this->prod *= n;
    return *this;
}

bool equal (const Res &r1, const Res &r2) {
    return r1.name == r2.name && r1.cons == r2.cons 
            && r1.prod == r2.prod && r1.price == r2.price;
}
