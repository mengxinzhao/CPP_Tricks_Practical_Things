#include <complex>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

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

int main() {
  Object<int, float, std::complex<float>> test(10, 0.3, {0.4, -1.0});
  test.print();
  auto string = test.toString();
  std::cout << string << std::endl;
}
