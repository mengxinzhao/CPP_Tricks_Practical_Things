//
// Created by Vesper on 6/8/18.
//

#include "config_builder.h"
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
using namespace std;

// If for each product I have a pre-defined supporting features and I need to
// build a configuration structure efficiently.
// Looking at chained-builder pattern
//
//        config_name, support_A, support_B, support_C
#define DEFINE_FEATURE(NAME)                                              \
  struct Feature##NAME {                                                  \
    static const bool value = true;                                       \
    void get() const { cout << "value: " << boolalpha << value << endl; } \
  };

DEFINE_FEATURE(A)
DEFINE_FEATURE(B)
DEFINE_FEATURE(C)
DEFINE_FEATURE(D)

#define DEFINE_CONFIG2(NAME, FeatureA) \
  auto config_##NAME = Config::create().set_feature(FeatureA{})();

#define DEFINE_CONFIG3(NAME, FeatureA, FeatureB, FeatureC) \
  auto config_##NAME =                                     \
      Config::create().set_feature(FeatureA{}).set_feature(FeatureB{}).set_feature(FeatureC{})();

#define DEFINE_CONFIG4(NAME, ...) auto config_##NAME = Config::create().set_feature(__VA_ARGS__)();

int main() {
  DEFINE_CONFIG2(NewProduc2, FeatureA);
  auto [begin, end] = config_NewProduc2->get_features();
  auto iterator = begin;
  while (iterator != end) {
    cout << typeid(*iterator).name() << endl;
    iterator->get();
    iterator++;
  }
  DEFINE_CONFIG3(NewProduc3, FeatureA, FeatureB, FeatureC);
  tie(begin, end) = config_NewProduc3->get_features();
  iterator = begin;
  while (iterator != end) {
    cout << typeid(*iterator).name() << endl;
    iterator->get();
    iterator++;
    // this is not gonna match because the real type is hidden
    if (typeid(*iterator) == typeid(FeatureB)) cout << "Feature B supported" << endl;
  }
  DEFINE_CONFIG4(TestProduct1, FeatureA{}, FeatureB{});
  DEFINE_CONFIG4(TestProduct2, FeatureA{}, FeatureB{}, FeatureC{}, FeatureD{});
  tie(begin, end) = config_TestProduct2->get_features();
  iterator = begin;
  while (iterator != end) {
    if (iterator->type() == typeid(FeatureC{})) {
      auto f_c = iterator->get<FeatureC>();
      cout << "Feature C supported" << endl;
      cout << typeid(*iterator).name() << endl;
    }
    iterator->get();
    iterator++;
  }
  return 0;
}
