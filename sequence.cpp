#include <algorithm>
#include <tuple>
#include <utility>
#include <iostream>

//A study on compile time sequence generation and computation
//
//
template <size_t N, typename ...Args>
void compile_time_print() {
    std::cout<<"Debug..."<<N<<std::endl;
}

// index sequence only
template <std::size_t ...N>
struct index_sequence
 {};

// generic form
template <std::size_t N, std::size_t ... Next>
struct index_sequence_impl : public index_sequence_impl<N-1U, N-1U, Next...>
 { };

// paritial specialization
template <std::size_t ... Next>
struct index_sequence_impl<0U, Next ... >
 { using type = index_sequence<Next ... >; };

template <std::size_t N>
using make_index_sequence = typename index_sequence_impl<N>::type;

// make_index_sequence<3>{} ==> index_sequence_impl<3>::type
// ==> matches index_sequence_impl<2, |2, Next...> Next not knowing
// ==> matches index_sequence_impl<1, |1, 2, Next...> 1, 2, Next... as the new Next
// ==> matches index_sequence_impl<0, |0, 1, 2, Next...>  0, 1, 2,  Next ...as the new Next
// ==> matches partial specializationj index_sequence_impl<0,Next...> = index_sequence<0,  1 ,  2>

// easy to read  verison
template <std::size_t N, std::size_t... Next>
auto make_index_sequence_impl() {
    // only one branch is considered. The other may be ill-formed
    if constexpr (N == 0) return index_sequence<Next...>(); // end case
    else return make_index_sequence_impl<N-1, N-1, Next...>(); // recursion
}

template <std::size_t N>
using make_index_sequence_2 = std::decay_t<decltype(make_index_sequence_impl<N>())>;

// generate a tuple of same type
template<typename T, std::size_t N>
struct tuple_n  {
    template <typename ...Args>
        using type = typename  tuple_n<T, N-1U> :: template type<T, Args...>;
};

//paritial specializatino
template<typename T>
struct tuple_n<T,0> {
    template <typename ...Args>
        using type = std::tuple<Args...>;
};

template<typename  T, std::size_t  N>
using tuple_of = typename tuple_n<T, N>::template type<>;

//tuple_of(int,3) == > matches tuple_n<int,3>::template  type<> args empty
//==> matches tuple_n<int,2>::template type<int> args..empty
//==> matches tuple_n<int,1>::template type<int,int...>
//==> matches tuple_n<int, 0>::template<int, int, int >::type == std::tuple<int, int, int>

//generate a number index sequence  that does square to the provided sequence
// the result is an index_sequence type not a tuple object!
//square_seq<5> = {0, 1, 4, 9, 16}

// When generate a sequence from sequence, use index_sequence<I, ...Next> as type
// and return tuple
// When generate a result from sequence, use regular size_t N as type
template <size_t ...N>
constexpr auto square_seq_impl(std::index_sequence<N...>);

//base. function template overloaded by none-template function
constexpr auto square_seq_impl(std::index_sequence<>){
    return std::tuple<>{};
}

// general form for recursion
template<std::size_t I, std::size_t ...Next>
constexpr auto square_seq_impl(std::index_sequence<I,Next...>){
    return  std::tuple_cat(std::make_tuple(I*I), square_seq_impl(std::index_sequence<Next...>{}));
}

template<std::size_t N>
constexpr auto square_seq_v = square_seq_impl(std::make_index_sequence<N>{});



int main() {
  constexpr int total_point = 64;
//(gdb) ptype  sequence1
//type = const struct std::integer_sequence<unsigned long, 0, 1, 2, 3 ...  60, 61, 62, 63>
//    [with _Tp = unsigned long] {
//  public:
//   static _Tp size(void);

  const auto sequence1 = std::make_index_sequence<total_point>();
  const auto sequence2 = make_index_sequence<total_point>();
  const auto sequence3 = make_index_sequence_2<total_point>();
  static_assert(std::is_same_v<decltype(sequence2), decltype(sequence3)>);
  const auto tuple_of_5_ints = tuple_of<int,5>();
  const auto tuple_of_0_ints = tuple_of<int,0>();
  const auto v0 = square_seq_v<0>; // (const std::__1::tuple<>) $0 = {}
  const auto v5 = square_seq_v<5>;
  static_assert(std::tuple_size<decltype(v0)>::value == 0);
  static_assert(std::tuple_size<decltype(v5)>::value == 5);

  return 0;
}
