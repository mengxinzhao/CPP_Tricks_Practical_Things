#include <iomanip>
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>

// A study note on forwarding parameters perfectly
int next_calculate(const int &t, const int &u) { return t * 4 * ((u ? u : 1) - 10); }

template <typename T, typename U>
auto next_calculate(const T &t, const U &u) {
  return t * 3.4 * ((u ? u : 1.0) - 10);
}

template <typename T, typename... Ts>
auto calculate(const T &t, Ts &&... args) {
  using ReturnType = std::common_type_t<T, Ts...>;
  ReturnType result = t + 5;
  if constexpr (sizeof...(args) > 0) {
    // fold expression
    result += next_calculate(std::forward<Ts>(args)...);
  }
  return result;
}

template <typename T>
const auto pi = (T)3.14159265358979323846264338327950288419716939937510582097494459230781640;

int main() {
  auto result1 = calculate(4);  // 4 + 5
  std::cout << result1 << std::endl;
  auto result2 = calculate(4, 1, 4.0);  // 4 + 5 + (1*4*((1.4-10))
  std::cout << std::setprecision(6) << result2 << std::endl;
  auto result3 = calculate(4, 1, 4);  // 4 + 5 + (1*4*(4-10))
  std::cout << result3 << std::endl;

  // const lvalue to next_calculate,  forward<> deduces to int &, not const int
  // & or const const int &, and pass on as const int &
  // const const
  auto result4 = calculate(4, 1, pi<int>);
  auto result5 = calculate(4, 1, pi<double>);

  auto &ref = pi<int>;  // const int &
  auto result6 = calculate(4, 1, ref);

  float almost_pi = 3.14;  // none-const lvalue
  // forward passes on as float&
  auto result7 = calculate(4, 1, almost_pi);

  auto &pi_ref = almost_pi;
  auto result8 = calculate(4, 1, pi_ref);

  // pointer works too
  auto result9 = calculate(4, 1, *std::make_shared<float>(4));

  // rvalue reference
  auto &&pi_rref = almost_pi;
  auto result10 = calculate(4, 1, pi_rref);
  return 0;
}
