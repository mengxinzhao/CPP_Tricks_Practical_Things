#include <algorithm>
#include <tuple>
#include <utility>
#include <iostream>
#include <functional>

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
// the result is a tuple object
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

//variable template
template<std::size_t N>
constexpr auto square_seq_v = square_seq_impl(std::make_index_sequence<N>{});

//iteration over tuples similar as for_each
//for_each_elem(tuple, function_to_apply, additional_args...)
// generic form
// 1st argument is tuple element, additional arguments are passed by
template <size_t Index=0, typename Tuple,
         size_t Size = std::tuple_size_v<std::decay_t<Tuple>>,
         typename Func, typename ...Args> //additional arguments passed to the callable
// it is either a specific return type or constexpr auto if you want to use auto
constexpr auto for_each_elem(Tuple &&tuple, Func &&func, Args &&...args) {
  if constexpr(Size > 0) {
    std::invoke(func,std::get<Index>(tuple),args...);
    if constexpr (Index+1 < Size) {
        for_each_elem<Index+1>(std::forward<Tuple>(tuple),
                    std::forward<Func>(func), std::forward<Args>(args)...);
    }
  }
}

//print all tuple elements in a single call
template <size_t Index = 0, typename Tuple,
          size_t Size = std::tuple_size_v<std::decay_t<Tuple>>>
void print_tuple(Tuple &&tuple){
  if constexpr (Size >0) {
    std::cout<<std::get<Index>(tuple) << " ";
    if constexpr (Index+1 < Size) {
      print_tuple<Index+1>(std::forward<Tuple>(tuple));
    }else {
      std::cout<<std::endl;
    }
  }
}



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
  auto v5 = square_seq_v<5>;
  //auto v6 = std::remove_const<decltype(v5)>(v5);
  static_assert(std::tuple_size<decltype(v0)>::value == 0);
  static_assert(std::tuple_size<decltype(v5)>::value == 5);

  print_tuple(v0);
  // using index sequence implies all members in the tuple is integer
  // const float constant =0.5 would compile but x will be rounded to integer
  const int constant = 5;
  print_tuple(v5);
  for_each_elem(v5,[&](auto &x) {x+=constant; } ) ;
  print_tuple(v5);

  //test with addtional args and standalone lambda
  for_each_elem(v5,[&](auto &x,int multiplier) {x*=multiplier; }, constant) ;
  print_tuple(v5);
  return 0;
}
