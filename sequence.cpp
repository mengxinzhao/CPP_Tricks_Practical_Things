#include <algorithm>
#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

// A study on compile time sequence generation and computation
//
//
template <size_t N, typename... Args> void compile_time_print() {
  std::cout << "Debug..." << N << std::endl;
}

// index sequence only
template <std::size_t... N> struct index_sequence {};

// generic form
template <std::size_t N, std::size_t... Next>
struct index_sequence_impl
    : public index_sequence_impl<N - 1U, N - 1U, Next...> {};

// paritial specialization
template <std::size_t... Next> struct index_sequence_impl<0U, Next...> {
  using type = index_sequence<Next...>;
};

template <std::size_t N>
using make_index_sequence = typename index_sequence_impl<N>::type;

// make_index_sequence<3>{} ==> index_sequence_impl<3>::type
// ==> matches index_sequence_impl<2, |2, Next...> Next not knowing
// ==> matches index_sequence_impl<1, |1, 2, Next...> 1, 2, Next... as the new
// Next
// ==> matches index_sequence_impl<0, |0, 1, 2, Next...>  0, 1, 2,  Next ...as
// the new Next
// ==> matches partial specializationj index_sequence_impl<0,Next...> =
// index_sequence<0,  1 ,  2>

// easy to read  verison
template <std::size_t N, std::size_t... Next> auto make_index_sequence_impl() {
  // only one branch is considered. The other may be ill-formed
  if constexpr (N == 0)
    return index_sequence<Next...>(); // end case
  else
    return make_index_sequence_impl<N - 1, N - 1, Next...>(); // recursion
}

template <std::size_t N>
using make_index_sequence_2 =
    std::decay_t<decltype(make_index_sequence_impl<N>())>;

// generate a tuple of same type
template <typename T, std::size_t N> struct tuple_n {
  template <typename... Args>
  using type = typename tuple_n<T, N - 1U>::template type<T, Args...>;
};

// paritial specializatino
template <typename T> struct tuple_n<T, 0> {
  template <typename... Args> using type = std::tuple<Args...>;
};

template <typename T, std::size_t N>
using tuple_of = typename tuple_n<T, N>::template type<>;

// tuple_of(int,3) == > matches tuple_n<int,3>::template  type<> args empty
//==> matches tuple_n<int,2>::template type<int> args..empty
//==> matches tuple_n<int,1>::template type<int,int...>
//==> matches tuple_n<int, 0>::template<int, int, int >::type == std::tuple<int,
// int, int>

// generate an array of
// array_of(1,2,3,4.0) ==> array<float,4>
template <typename... Ts>
constexpr auto make_array(Ts &&... ts)
    -> std::array<std::common_type_t<Ts...>, sizeof...(ts)> {
  // need to promote all elem type to the common type
  using ElemType = std::common_type_t<Ts...>;
  return {std::forward<ElemType>(ts)...};
}
// index_sequence<...Ints> is a type but when used in function template as
// argument don't use index_sequence<...Ints>{}. Will not compile. When used as
// an argument to a function template, the parameter pack Ints can be deduced
// and used in pack expansion

// generate an array of type T from index sequence
template <typename T, std::size_t N, std::size_t... Is>
constexpr auto array_of_impl(std::index_sequence<Is...>) {
  return std::array<T, N>{{Is...}};
}

template <typename T, std::size_t N> constexpr auto array_of() {
  return array_of_impl<T, N>(std::make_index_sequence<N>{});
}
// generate an array of Type T, size N and apply func on every index
// enable only when F is a function
template <typename T, std::size_t N, typename F, std::size_t... Is>
constexpr auto array_of_impl(F &&func, std::index_sequence<Is...>) {
  // to avoid -Wc++11-narrowing
  // using ElemType = std::common_type_t<T,std::size_t>;
  // return std::array<ElemTyp,N>{ {func(Is)...} };
  // force T type
  // return std::array<T, N>{{func(static_cast<T>(Is))...}};
  return std::array<T, N>{{func(Is)...}};
}

// std::is_function will not work with lamda.
// Types like std::function, lambdas, classes with overloaded operator() and
// pointers to functions don't count as function types.

// don't know why is_invocable doesn't compile in clang. Got error message:
// no template named 'is_invocable' in namespace 'std'; did you mean
// '__invokable'?
// std::is_invocable works with gcc version 7.4.0 (Ubuntu 7.4.0-1ubuntu1~18.04)
//  with -std=c++17
template <typename T, std::size_t N, typename F,
          typename = std::enable_if_t<std::is_invocable<F, T>::value>>
constexpr auto array_of(F &&func) {
  return array_of_impl<T, N>(std::forward<F>(func),
                             std::make_index_sequence<N>{});
}

// generate an array of Type T, size N and value V
template <typename T, std::size_t N> constexpr auto array_of(T &&v) {
  std::array<T, N> arr{};
  arr.fill(v);
  return arr;
}

// generate a number index sequence  that does square to the provided sequence
// the result is a tuple object
// square_seq<5> = {0, 1, 4, 9, 16}

// When generate a sequence from sequence, use index_sequence<I, ...Next> as
// type and return tuple When generate a result from sequence, use regular
// size_t N as type
template <size_t... N>
constexpr auto square_seq_impl(std::index_sequence<N...>);

// base. function template overloaded by none-template function
constexpr auto square_seq_impl(std::index_sequence<>) { return std::tuple<>{}; }

// general form for recursion
template <std::size_t I, std::size_t... Next>
constexpr auto square_seq_impl(std::index_sequence<I, Next...>) {
  return std::tuple_cat(std::make_tuple(I * I),
                        square_seq_impl(std::index_sequence<Next...>{}));
}

// variable template
template <std::size_t N>
constexpr auto square_seq_v = square_seq_impl(std::make_index_sequence<N>{});

// iteration over tuples similar as for_each
// for_each_elem(tuple, function_to_apply, additional_args...)
// generic form
// 1st argument is tuple element, additional arguments are passed by
template <size_t Index = 0, typename Tuple,
          size_t Size = std::tuple_size_v<std::decay_t<Tuple>>, typename Func,
          typename... Args> // additional arguments passed to the callable
// it is either a specific return type or constexpr auto if you want to use auto
constexpr auto
for_each_elem(Tuple &&tuple, Func &&func, Args &&... args) {
  if constexpr (Size > 0) {
    std::invoke(func, std::get<Index>(tuple), args...);
    if constexpr (Index + 1 < Size) {
      for_each_elem<Index + 1>(std::forward<Tuple>(tuple),
                               std::forward<Func>(func),
                               std::forward<Args>(args)...);
    }
  }
}

// print all tuple elements in a single call
template <size_t Index = 0, typename Tuple,
          size_t Size = std::tuple_size_v<std::decay_t<Tuple>>>
void print_tuple(Tuple &&tuple) {
  if constexpr (Size > 0) {
    std::cout << std::get<Index>(tuple) << " ";
    if constexpr (Index + 1 < Size) {
      print_tuple<Index + 1>(std::forward<Tuple>(tuple));
    } else {
      std::cout << std::endl;
    }
  }
}

// generate an array of x-d coordinates from index sequence for a x-d grid
// i.e [(0,0), (0,1), (0,2), (0,3), (1,0),(1,1), (1,2), (1,3)...] for a (4x4)
// template parameter: D dimension,  Sz  grid size, N index_sequence<N>
template <size_t D, size_t Sz, size_t N, size_t... Is>
constexpr auto generate_xd_coordinates_impl(std::index_sequence<Is...>) {
  return std::array<std::array<size_t, D>, N>{{{Is % Sz, (Is / Sz) % Sz}...}};
}

template <size_t D, size_t Sz, size_t N>
constexpr auto generate_xd_coordinates() {
  return generate_xd_coordinates_impl<D, Sz, N>(std::make_index_sequence<N>{});
}

int main() {
  // p idx_seq
  //(const std::__1::integer_sequence<unsigned long, 1, 2, 4, 5>) $0 = {}
  const auto idx_seq = std::index_sequence<1, 2, 4, 5>{};
  std::cout << idx_seq.size() << std::endl;
  constexpr int total_point = 64;
  //(gdb) ptype  sequence1
  // type = const struct std::integer_sequence<unsigned long, 0, 1, 2, 3 ...
  // 60, 61, 62, 63>
  //    [with _Tp = unsigned long] {
  //  public:
  //   static _Tp size(void);

  const auto sequence1 = std::make_index_sequence<total_point>();
  const auto sequence2 = make_index_sequence<total_point>();
  const auto sequence3 = make_index_sequence_2<total_point>();
  static_assert(std::is_same_v<decltype(sequence2), decltype(sequence3)>);
  const auto tuple_of_5_ints = tuple_of<int, 5>();
  const auto tuple_of_0_ints = tuple_of<int, 0>();
  const auto v0 = square_seq_v<0>; // (const std::__1::tuple<>) $0 = {}
  auto v5 = square_seq_v<5>;
  // auto v6 = std::remove_const<decltype(v5)>(v5);
  static_assert(std::tuple_size<decltype(v0)>::value == 0);
  static_assert(std::tuple_size<decltype(v5)>::value == 5);

  print_tuple(v0);
  // using index sequence implies all members in the tuple is integer
  // const float constant =0.5 would compile but x will be rounded to integer
  const int constant = 5;
  print_tuple(v5);
  for_each_elem(v5, [&](auto &x) { x += constant; });
  print_tuple(v5);

  // test with addtional args and standalone lambda
  for_each_elem(v5, [&](auto &x, int multiplier) { x *= multiplier; },
                constant);
  print_tuple(v5);

  const auto ar1 = make_array(1, 2, 3, 4, 5);
  const auto ar2 = make_array(1, 2, 3, 4, 5.4);

  const auto ar3 = array_of<int, 3>();
  const auto ar4 = array_of<int, 0>();
  const auto ar5 = array_of<int, 5>(9);
  const auto ar6 =
      array_of<int, 5>([=](auto x) -> decltype(x) { return x * x; });
  const auto ar7 = std::array<tuple_of<int, 2>, 2>{std::make_tuple(3, 4),
                                                   std::make_tuple(5, 6)};
  const auto coord_2d = generate_xd_coordinates<2, 16, 16 * 16>();
  for (const auto &elm : coord_2d) {
    std::cout << elm[0] << " " << elm[1] << std::endl;
  }
  return 0;
}
