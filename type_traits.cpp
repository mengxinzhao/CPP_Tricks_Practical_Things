//
// Created by Vesper on 6/8/18.
//

// code note for modern c++ metaprogramming
// https://www.youtube.com/watch?v=JSR8YBhW_uM&t=5077s
#include <iostream>
#include <type_traits>

using namespace std;
struct HasX {int x;};
struct HasY {int y;};

template <typename T, typename=void >
constexpr  bool has_x = false;

template <typename T>
constexpr  bool has_x <T, void_t< decltype(T::x)>> =true;

template <typename T, typename = void>
constexpr  bool has_x_of = false;

template<typename T>
constexpr bool has_x_of <T, decltype(T::x)> = true;

//Typelist
template <typename T, typename ...Types>
constexpr  bool ContainsType = false;

// https://en.cppreference.com/w/cpp/language/parameter_pack
// A pattern followed by an ellipsis, in which the name of at least one parameter pack appears at least once,
// is expanded into zero or more comma-separated instantiations of the pattern, where the name of the parameter
// pack is replaced by each of the elements from the pack, in order.
template<typename T, typename ...Rest>
constexpr  bool ContainsType<T,T,Rest...> = true;

// recursively calling itself

template<typename T, typename  J, typename ...Rest>
constexpr  bool ContainsType<T,J,Rest...> = ContainsType<T,Rest...>;

int main(){
    static_assert(has_x<HasX>);
    static_assert(!has_x<HasY>);
    static_assert(has_x_of<HasX,int>);
    static_assert(!has_x_of<HasY,int>);
    static_assert(!has_x_of<HasX,float>);

    static_assert(ContainsType<int, float, double, int, void>);
    static_assert(!ContainsType<char, float, double, int, void>);
    return 0;
}