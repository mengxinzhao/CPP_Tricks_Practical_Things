//
// Created by Vesper on 7/13/18.
//

// reading note from https://msdn.microsoft.com/en-us/library/dn919277.aspx
// use user defined literals to make variable clearer in conveying meanings
// double a_person_height = 1.60
// better be expressed as double a_person_height=1.60m or
// double a_person_height = 160cm

#include <iostream>

using namespace std;
class Height {
    long double height_in_cm{0.0};
    explicit  Height (long double h ):height_in_cm(h) {}
    friend Height operator""_cm(long double h);
    friend Height operator""_m(long double h);
    friend Height operator""_in(long double h);
    friend Height operator""_ft(long double h);
    friend ostream& operator<<(ostream& os, const Height& ht);
public:
    long double get_height_in_cm() const {return height_in_cm;}
    bool operator < (Height &other) {
        return height_in_cm < other.get_height_in_cm();
    }
};

Height operator""_cm(long double h) {return Height(h);}
Height operator""_m(long double h) { return Height(h * 100);}
Height operator""_in(long double h) {return Height(h * 2.54);}
Height operator""_ft(long double h) {return Height(h * 12 * 2.54);}
ostream& operator<<(ostream& os, const Height& ht) {
    os<<ht.get_height_in_cm()<<"cm" ;
    return os;
}


template <char... Digits>
struct binary_literal_impl;

// partial specialization
template <char first, char ... Rest>
struct binary_literal_impl<first, Rest...>
{
    static constexpr unsigned long long to_ulonglong()
    {
        static_assert(first == '0' || first == '1', "not a valid binary number");
        if (first == '0')
            return  binary_literal_impl<Rest...>::to_ulonglong();
        else
            return (1ULL << sizeof...(Rest))
                   | binary_literal_impl<Rest...>::to_ulonglong();
    }
};


// Base case: No digits, so return 0...
template <>
struct binary_literal_impl<>
{
    static constexpr unsigned long long to_ulonglong()
    {
        return 0;
    }
};


// literal template
// If the literal operator is a template, it must have an empty parameter list and can have only one template parameter,
// which must be a non-type template parameter pack with element type char
template <char... Digits>
constexpr unsigned long long operator ""_bin()
{

    return binary_literal_impl<Digits...>::to_ulonglong();
}

int main(){
    auto height1 = Height{1.89_m};
    std::cout<<height1<<std::endl;
    auto height2 = Height{6.1_ft};
    std::cout<<height2<<std::endl;
    std::cout<<std::boolalpha << (height1< height2 )<< std::endl;

    auto bits = 01011_bin;
    std::cout<<"bits: "<< hex<<bits<<std::endl;
    bits = 1011_bin;
    std::cout<<"bits: "<< hex<<bits<<std::endl;
    bits = 0_bin;
    std::cout<<"bits: "<< hex<<bits<<std::endl;
    bits = 2011_bin;
    std::cout<<"bits: "<< hex<<bits<<std::endl;

}