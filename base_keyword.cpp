//
// Created by Vesper on 6/11/18.
//
#include <iostream>
// confused a lot by other people using base::base
// turns out base is just an alias
struct A {
  A(int) { std::cout << "in A ctor" << std::endl; }
  virtual ~A() = default;

  virtual void update() { std::cout << "in A update" << std::endl; }
};

struct B : A {
  using Base = A;  // 'base' is a type alias for 'A'

  using Base::Base;  // inheriting constructor
  // equivalent to B( int v ) : base(v) {}
  B() : Base(5) { std::cout << "in B ctor" << std::endl; }

  virtual void update() override {
    Base::update();  // same as A::update() ;
    std::cout << "in B update\n" << std::endl;
  }
};
int main() {
  B* b_ptr = new B();
  b_ptr->update();
  delete b_ptr;
}
