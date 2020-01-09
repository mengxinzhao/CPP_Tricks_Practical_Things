#include <iostream>

#include <numeric>
#include <tuple>
#include <type_traits>
#include <utility>
#include <array>

template <typename T, T Begin, T End, T Step = 1,
          typename std::enable_if<std::is_integral<T>::value>::type* = nullptr>
class IntegerRange {
  // forware declarations
  struct Iterator;

 public:
  using ValueType = T;
  using RangeType = std::array<T, (End - Begin + Step-1)/Step>;
  IntegerRange() {
    static_assert(Step > 0);
    static_assert(Begin <= End);
  }
  Iterator begin() {
    // static_assert(Begin <= std::numeric_limits<T>::max() && Begin >=
    // std::numeric_limits<T>::min());
    return Iterator(Begin);
  }
  Iterator end() {
    // static_assert(End <= std::numeric_limits<T>::max() && End >= std::numeric_limits<T>::min());
    return Iterator(End);
  }
  bool inRange(T v) { return (v <= End && v >= Begin && !(v % Step)); }

  RangeType values() {
      RangeType ret;
      size_t i=0;
      for (auto iter = begin(); iter<end(); ++iter){
          ret[i++] = *iter;
      }
      return ret;
  }
 private:
  // Simple nested iterator to meet range loop interface
  struct Iterator {
    T _value;
    Iterator(T v) : _value(v) {}
    operator T() const { return _value; }
    operator T&() { return _value; }
    T operator*() const { return _value; }
    Iterator& operator++() {
      _value += Step;
      return *this;
    }
  };
};

class ValueInRange : public IntegerRange<int, -19, 20 + 1> {
 public:
  explicit ValueInRange(ValueType v) {
    if (inRange(v)) {
      _value = v;
    } else {
      throw std::runtime_error("Value not in range");
    }
  }

  ValueInRange() = default;

  ValueInRange(const ValueInRange& v) = default;

  ValueInRange& operator=(const ValueInRange& v) = default;
  ValueType get() const { return _value; }

 protected:
  ValueType _value = begin();
};

int main() {
  for (const auto& v : IntegerRange<int, 1, 5, 2>()) std::cout << v << std::endl;
  auto array = IntegerRange<int, 0,5>().values();
  for (auto v : array)
      std::cout<<v<< " " ;
  std::cout<<std::endl;
  std::cout<<"step =2 "<<std::endl;
  auto array2 = IntegerRange<int, 0,5,2>().values();
  for (auto v : array2)
      std::cout<<v<< " " ;
  std::cout<<std::endl;
  std::cout<<"step = 3 "<<std::endl;
  auto array3 = IntegerRange<int, 0,5,3>().values();
  for (auto v : array3)
      std::cout<<v<< " " ;
  std::cout<<std::endl;
  std::cout<<"######"<<std::endl;
  auto myRange = IntegerRange<int, 0, 5, 2>();
  std::cout << myRange.inRange(2) << std::endl;
  auto myRange1 = ValueInRange(4);
  std::cout << myRange1.get() << std::endl;
  for (const auto& v : myRange1) std::cout << v << std::endl;
  ValueInRange myRange2;
  std::cout << myRange2.get() << std::endl;
  auto myRange3 = myRange2;
  std::cout << myRange3.get() << std::endl;
  //auto myRange4 = ValueInRange(100);
  std::tuple<int, char, int> v = std::make_tuple<int, char, int>(3, '4', 5);
  auto another = v;
  //auto bad1 = IntegerRange<int, 4,2,1>();
  //auto bad2 = IntegerRange<int, 4,2,0>();
  auto empty= IntegerRange<int, 3, 3>();
  auto empty_array = empty.values();
  return 0;
}
