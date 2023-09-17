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

    if (r1.get_name() != r2.get_name()) {
        throw std::logic_error ("Can't join distinct resources.");
    }

    Res sum;

    sum.set_name(r1.get_name());
    sum.set_cons(r1.get_cons() + r2.get_cons());
    sum.set_prod(r1.get_prod() + r2.get_prod());
    sum.set_price((r1.get_price() < r2.get_price()) ? r1.get_price() : r2.get_price());

    return sum;
}

std::weak_ordering operator<=> (const Res& r1, const Res& r2) noexcept {
    if (r1.get_name() < r2.get_name())
        return std::weak_ordering::less;
    else if (r1.get_name() > r2.get_name())
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
    return r1.get_name() == r2.get_name() 
        && r1.get_cons() == r2.get_cons()
        && r1.get_prod() == r2.get_prod() 
        && r1.get_price() == r2.get_price();
}
