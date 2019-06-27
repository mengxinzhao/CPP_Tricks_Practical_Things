#include <complex>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
// use fold expression  and initializer_list o do nicer things
// elements in initializer_list are  evaluated in the order they appear
template <typename... Ts> class Object {
public:
  Object(Ts... args) { members = std::make_tuple(std::forward<Ts>(args)...); }

  void print() {
    printImpl(std::make_index_sequence<std::tuple_size_v<decltype(members)>>{});
  }

  std::string toString() {
    return toStringImpl(
        std::make_index_sequence<std::tuple_size_v<decltype(members)>>{});
  }
  std::tuple<Ts...> members;

protected:
  template <size_t... Is> void printImpl(std::index_sequence<Is...>) {
    std::initializer_list<int>{
        // comma fold. The value of the empty pack is void ()
        // comma as operator, built-in type is return void.
        // Use comma operator with  (expression1, expression2 )
        // provide each member has a ostream <<  overload it should print
        // members nicely
        (std::cout << std::get<Is>(members) << " ", 0)...};
    std::cout << std::endl;
  }

  template <size_t... Is> std::string toStringImpl(std::index_sequence<Is...>) {
    std::stringstream ss;
    bool noComma = true;
    std::initializer_list<bool>{
        (ss << (noComma ? "" : ", ") << std::get<Is>(members),
         noComma = false)...};
    return ss.str();
  }
};

// basic operation. Use initialization list instead of heavy index_sequence
template <typename... Ts>
auto sum(const Ts &... t) -> typename std::common_type_t<Ts...> {
  typename std::common_type_t<Ts...> result{};
  std::initializer_list<int>{(result += t, 0)...};
}

template <typename T, typename... Ts> auto min(const T &t, const Ts &... ts) {
  using CommonType = typename std::common_type_t<T, Ts...>;
  auto result = static_cast<CommonType>(t);
  std::initializer_list<int>{(((CommonType)ts < (CommonType)result
                                   ? result = static_cast<CommonType>(ts)
                                   : result),
                              0)...};
  return result;
}

int main() {
  Object<int, float, std::complex<float>> test(10, 0.3, {0.4, -1.0});
  test.print();
  auto string = test.toString();
  std::cout << string << std::endl;

  std::cout << sum(1, 0.4, 2.4, 90UL) << std::endl;
  auto result1 = min(34, 34UL, 34.0f, 34.0d);
  static_assert(std::is_same_v<decltype(result1), double>);
  std::cout << std::setprecision(6) << result1 << std::endl;
  auto result2 = min(35, 34UL, (short)33, 32.3, (char)32);
  static_assert(std::is_same_v<decltype(result2), double>);
  std::cout << std::setprecision(6) << result2 << std::endl;
  std::cout << std::setprecision(6)
            << min(35, 34UL, (short)33, 32.3, (char)32.3)
            << std::endl; //  should  be 32
  return 0;
}
