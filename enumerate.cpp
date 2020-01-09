#include <cstddef>
#include <iostream>
#include <iterator>
#include <vector>

template <typename Iterable>
class enumerate_object {
 private:
  Iterable _iter;
  std::size_t _size;
  decltype(std::begin(_iter)) _begin;
  const decltype(std::end(_iter)) _end;

 public:
 explicit  enumerate_object(Iterable &&iter)
      : _iter(iter), _size(0), _begin(std::begin(iter)), _end(std::end(iter)) {}

  const enumerate_object& begin() const { return *this; }
  const enumerate_object& end() const { return *this; }

  bool operator!=(const enumerate_object&) const { return _begin != _end; }

  void operator++() {
    ++_begin;
    ++_size;
  }

  auto operator*() const -> std::pair<std::size_t, decltype(*_begin)> { return {_size, *_begin}; }
};

template <typename Iterable>
auto enumerate(Iterable&& iter) {
  return enumerate_object<Iterable>{std::forward<Iterable>(iter)};
}

int main() {
  std::vector<double> vec = {1., 2., 3., 4., 5.};
  for (auto&& [index, value] : enumerate(vec)) {
    std::cout << "value: " << value << std::endl;
    std::cout << "index: " << index << std::endl;
  }
}
