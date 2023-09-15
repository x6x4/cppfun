#include "res.h"
#include <algorithm>
#include <compare>
#include <iterator>
#include <stdexcept>
#include <vector>


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

/*std::weak_ordering operator<=> (const Res& r1, const Res& r2) noexcept {
    if (r1.name < r2.name)
        return std::weak_ordering::less;
    else if (r1.name > r2.name)
        return std::weak_ordering::greater;
    else 
        return std::weak_ordering::equivalent;
};*/

int Res::get_profit () noexcept {
    return (this->prod - this->cons)*this->price*7;
}

void Res::operator* (int n) noexcept {
    this->cons *= n;
    this->prod *= n;
}


//  CLASS Res_Table


/**
 * @brief       Add resource to table
 * @param       r  Resource 
 * @exception   std::runtime_error  Duplicate resource
 */
auto Res_Table::operator+= (Res r) {

    vec.resize();

    auto p = find (r.name);
    if (p.first)
        throw std::runtime_error ("Duplicate resource");
    else 
        ;

    return *this;
}

/**
 * @brief   Check table state
 * @return  Table state as integer
 */

int Res_Table::check () noexcept {
    return vec.check();
}

/**
 * @brief       Find resource by name
 * @param       name  Resource name
 * @return      Pair <is_found, matching place>
 */
std::pair<bool, Res> Res_Table::find (std::string name) noexcept {
    /*auto range = std::equal_range(vec.begin(), vec.end(), name);
    
    std::pair<bool, Res> p;
    p.second = *range.first;
    p.first = !(range.first == range.second);

    return p;*/
}

/**
 * @brief       Find resource by name
 * @param       name  Resource name
 * @return      Resource ref
 * @exception   std::runtime_error Name not found
 */

auto Res_Table::operator[](std::string name) {
    auto p = find (name);
    if (p.first)
        return p.second;
    else 
        throw std::runtime_error ("Name not found");
}

/**
 * @brief       Set resource name
 * @param       old_name  Old resource name
 * @param       new_name  New resource name
 */

void Res_Table::set (std::string old_name, std::string new_name) {
    auto p = (*this)[old_name];
    p.name = new_name;
}

/**
 * @brief       Delete resource from table
 * @param       r  Resource 
 * @exception   std::runtime_error  Empty table
 */

void Res_Table::del (Res r) {

    if (check() == EMPTY)
        throw std::runtime_error ("Empty table");

    auto p = (*this)[r.name];

}


/**
 * @brief       Get week profit for all resources.
 */

std::size_t Res_Table::get_profit () {

    std::size_t sum = 0;

    for (auto r : this->vec) {
        sum += r.get_profit();
    }

    return sum;
}

/**
 * @brief       Increase turnout for all resources 
                by the specified number of times
 * @param       n  Times 
 */
void Res_Table::operator* (std::size_t n) {
    for (auto r : this->vec) {
        r * n;
    }
}