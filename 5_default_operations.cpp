#include <iostream>
#include <utility>
#include <vector>

using namespace std;
// codes from youtube
// https://en.cppreference.com/w/cpp/language/rule_of_three
// Because the presence of a user-defined destructor, copy-constructor, or copy-assignment operator
// prevents implicit definition of the move constructor and the move assignment operator, any class
// for which move semantics are desirable, has to declare all five special member functions:

// Classes that have custom destructors, copy/move constructors or copy/move assignment operators
// should deal exclusively with ownership (which follows from the Single Responsibility Principle)
//
// When a base class is intended for polymorphic use, its destructor may have to be declared public
// and virtual. This blocks implicit moves (and deprecates implicit copies), and so the special
// member functions have to be declared as defaulted

class DefaultOperations {
 public:
  DefaultOperations() noexcept { cout << "Default Ctor" << endl; }  // No value initialization
  DefaultOperations(DefaultOperations const& rhs) : a{rhs.a}, b{rhs.b} {
    cout << "Default copy ctor" << endl;
  }
  DefaultOperations& operator=(DefaultOperations const& rhs) {
    a = rhs.a;
    b = rhs.b;
    cout << "Default assignment" << endl;
    return *this;
  }
  DefaultOperations(DefaultOperations&& rhs) noexcept : a{move(rhs.a)}, b{move(rhs.b)} {
    cout << "default rvalue copy ctor" << endl;
  }
  DefaultOperations& operator=(DefaultOperations&& rhs) noexcept {
    a = move(rhs.a);
    b = move(rhs.b);
    cout << "default assign rvalue " << endl;
    return *this;
  }
  ~DefaultOperations() noexcept {}

 private:
  int a = 1000;
  vector<int> b = {1, 2, 3, 4, 5, 6};
  // because of the const, the class it not copy assignable but movable
};

int main() {
  DefaultOperations test_1;
  DefaultOperations test_2(test_1);
  DefaultOperations test_3 = test_2;
  {
    DefaultOperations test_4(move(test_1));  // test_1 vector field becomes empty
    test_1 = move(test_2);                   // test_1 gets test_2 stuff. test_2 becomes empty
  }
  return 0;
}
