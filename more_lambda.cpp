#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <utility>
#include <variant>
// Even more interesting things about lambda from c++ weekly with Jason Tuerner

// inheriting from lambdas
// lambda is itself a type and can be inherited

template <typename L1, typename L2>
struct S : L1, L2 {
  S(L1 l1, L2 l2) : L1(std::move(l1)), L2(std::move(l2)) {}
  using L1::operator();
  using L2::operator();
};

template <typename L1, typename L2>
auto make_combined(L1 &&l1, L2 &&l2) {
  // Use the decay to produce the resulting type as if the type was passed by
  // value as an argument.
  return S<std::decay_t<L1>, std::decay_t<L2>>(std::forward<L1>(l1), std::forward<L2>(l2));
}

// use as a visitor to visit a range of things
template <typename... Ts>
struct Visitor : Ts... {
  Visitor(Ts &&... ts) : Ts(std::forward<Ts>(ts))... {}
  using Ts::operator()...;
};

// class template deduction guide.==>actually not needed
// template <typename... Ts>
// Visitor(Ts...)->Visitor<std::decay_t<Ts>...>;

int main() {
  auto l1 = []() { return 4; };
  auto l2 = [](int i) { return i * 4; };

  auto combined1 = make_combined(l1, l2);
  // c++17 template auto deduction
  auto combined2 = S(l1, l2);
  std::cout << combined1() << std::endl;
  std::cout << combined1(10) << std::endl;
  std::cout << combined2(10) << std::endl;

  auto combined3 = make_combined([]() { return 4; }, [](float &&a) { return a * 0.8; });
  std::cout << combined3(0.4) << std::endl;

  std::array<std::variant<float, int>, 5> data{3.2f, 2.0f, 2, 0.4f, 3};
  int sum_int = 0;
  float sum_float = 0;
  // sum int and float seperately
  Visitor v{[&sum_int](const int v) { sum_int += v; },
            [&sum_float](const float f) { sum_float += f; }};
  std::for_each(data.cbegin(), data.cend(), [&v](const auto &elem) { std::visit(v, elem); });
  std::cout << sum_int << std::endl;
  std::cout << sum_float << std::endl;

  // generic lambda with constexpr
  std::common_type_t<decltype(sum_int), decltype(sum_float)> sum_common = 0;
  sum_int = 0;
  sum_float = 0;
  auto v2 = [&sum_int, &sum_float, &sum_common](const auto &elem) {
    if constexpr (std::is_same_v<float, decltype(elem)>) {
      sum_float += elem;
      sum_common += elem;
    } else if constexpr (std::is_same_v<std::string, decltype(elem)> ||
                         std::is_convertible_v<std::string, decltype(elem)>) {
      sum_common += (int)'a';
    } else {
      sum_int += elem;
      sum_common += elem;
    }
  };
  for_each(data.cbegin(), data.cend(), [&v2](const auto &elem) { std::visit(v2, elem); });
  std::cout << sum_common << std::endl;

  // data generated at run time works with generic lamda too !
  std::array<std::variant<float, int, std::string>, 5> data2{};
  for (int i = 0; i < 5; i++) {
    if (!i % 3) data2[i] = i;
    if (i % 3 == 1) data2[i] = 0.5f;
    if (i % 3 == 2) data2[i] = std::to_string(i);
  }
  sum_int = 0;
  sum_float = 0;
  sum_common = 0;

  for_each(data2.cbegin(), data2.cend(), [&v2](const auto &elem) { std::visit(v2, elem); });
  std::cout << sum_int << std::endl;
  std::cout << sum_float << std::endl;
  std::cout << sum_common << std::endl;

  return 0;
}
