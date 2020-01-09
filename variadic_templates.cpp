//
// Created by Vesper on 6/11/18.
//

// codes from https://www.youtube.com/watch?v=R1G3P5SRXCw&t=1179s
// one template at the time of instantiation, can only look up the templates
// that are defined before it. It doesn't look up the all definition.
// So need to make sure the special cases are all defined before
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

template <typename T>
void P(T x) {
  std::cout << x << ' ';
}

void foo(char a) {
  P(3);
  P(a);
}

template <typename... Args>
void foo(int a, Args... args) {
  foo(args...);
  P(a);
}

template <typename... Args>
void foo(char a, Args... args) {
  P(a);
  foo(args...);
}

template <typename T, typename... Ts>
struct ManyTypeMembers {
  T primary;
  std::tuple<Ts...> secondary;

  ManyTypeMembers(const T &t, Ts &&... ts) : primary(t) {
    secondary = std::make_tuple(std::forward<Ts>(ts)...);
  }
};

float extra_stuff(int i, float f, std::string s) {
  std::cout << s << std::endl;
  return i * 2.0 + f + 3.5;
}

template <typename Tuple, size_t... Is>
auto middle_man(int i, const Tuple &tpl, std::index_sequence<Is...>) {
  if constexpr (sizeof...(Is) > 0)
    return extra_stuff(i, std::get<Is>(tpl)...);
  else
    std::cout << "no extra stuff" << std::endl;
}

template <typename Tuple>
auto fun(const int &i, const Tuple &tpl) {
  return middle_man(i, tpl,
                    std::make_index_sequence<std::tuple_size_v<std::decay_t<decltype(tpl)>>>{});
}

enum COLOR { RED, MULTI };

struct Red {
  int width = 10;
};

struct Blue {
  int width = 11;
};

struct Yellow {
  int width = 12;
};

template <COLOR color, typename... ParamType>
class Draw {
 public:
  Draw() = default;
  Draw(ParamType... params) : _params{params...} {}
  virtual void draw() = 0;

 protected:
  std::tuple<ParamType...> _params;
};

class DrawRed : public Draw<RED, Red> {
 public:
  void draw() { std::cout << "drawing red..." << std::get<0>(_params).width << std::endl; }
};

class DrawMulti : public Draw<MULTI, Blue, Yellow> {
 public:
  void draw() {
    std::cout << "drawing blue..." << std::get<0>(_params).width << std::endl;
    std::cout << "drawing yellow..." << std::get<1>(_params).width << std::endl;
  }
};

int main() {
  foo('1', '2', 48, '4', '5');
  // output
  // 1 2 3 5 52 48
  // call sequence P('1')-->P('2')-->P('4','5') --> P(48)
  // P('4','5') --> P((int)'5') -- > P((int)'4')
  // P((int)'5')--> P(3),P(5(ascii))
  // P((int)'4') -->  P(4(ascii))

  ManyTypeMembers<int, double, std::string> t1{1, 3.4, "hahahahaha"};
  ManyTypeMembers<int> t2{1};
  std::cout << t1.primary << std::endl;
  std::cout << std::tuple_size_v<decltype(t2.secondary)> << std::endl;

  auto result = fun(4, t1.secondary);
  fun(4, t2.secondary);

  auto draw_multi = DrawMulti();
  draw_multi.draw();
  auto draw_red = DrawRed();
  draw_red.draw();
  return 0;
}
