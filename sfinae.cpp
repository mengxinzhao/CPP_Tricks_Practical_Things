#include <iostream>
#include <type_traits>
#include <string>
#include <experimental/type_traits>

//using traits to statically dispatch functions
//
using namespace std;
template<typename T>
typename std::enable_if_t<std::is_integral<T>::value,int> increment(T t){
    return ++t;
}
template<typename T>
typename std::enable_if_t<std::is_floating_point<T>::value,float> increment(T t){
    t += 1.01;
    return t;
}

// this also works

//template<typename T>
//class IsClassT {
//  private:
//    typedef char One;
//    typedef struct { char a[2]; } Two;
//    template<typename C> static One test(int C::*);
//    // Will be chosen if T is anything except a class.
//    template<typename C> static Two test(...);
//  public:
//    enum { Yes = sizeof(IsClassT<T>::template test<T>(0)) == 1 };
//    enum { No = !Yes };
//};

//When IsClassT<int>::Yes is evaluated, 0 cannot be converted to int int::* because int
// is not a class, so it can't have a member pointer. If SFINAE didn't exist, then you
// would get a compiler error, something like '0 cannot be converted to member pointer
// for non-class type int'. Instead, it just uses the ... form which returns Two, and thus
// evaluates to false, int is not a class type.
//

template<typename T>
class IsClassT {
    template<typename C> static bool test(int C::*) {return true;}
    template<typename C> static bool test(...) {return false;}
public:
    static bool value;
};

template<typename T>
bool IsClassT<T>::value=IsClassT<T>::test<T>(0);



class TestClass {
    int test_a;
    float foo;
public:
    typedef float target_type;
    void print(TestClass &, unsigned N);
    float do_something_1(int one, float two);
    float do_something_2(float one, float two);
};

// check if a class has a public print function
template<class T> struct has_print
{
    typedef char (&Yes)[1];
    typedef char (&No)[2];
    
    template<class U>
    static Yes test(U * data, T* arg1 = 0, typename std::enable_if_t<std::is_void<decltype(data->print(*arg1, 1u))>::value> * = 0);
    static No test(...);
    static const bool value = (sizeof(Yes) == sizeof(has_print::test((typename std::remove_reference<T>::type*)nullptr)));
};

// check if a class has a type
template <typename T>
using has_typedef_target_type_t = typename T::target_type;

struct TypeA {
   TypeA(int _value):value(_value) {}
   int value;
};

struct TypeB {
   TypeB(int _value):value(_value) {}
   int value;
};

template <typename T, std::enable_if_t<std::is_same<T, TypeA>::value, T>* = nullptr> 
T& add1 (T & a){
	a.value++;
	return a;
}

int main(){
    class A{
        int a;
        void print(A& _A, unsigned int N=10) {}
    };
    class B {};
  int a = 10;
  int &b = a;
  int *p = &a;
  cout<<increment(1.0)<<endl;
  cout<<increment(1)<<endl;
  //cout<<increment(p)<<endl;   // no match functions
  cout<<increment(b)<<endl;   // no match functions
  cout<<IsClassT<string>::value<<endl;
  cout<<IsClassT<float>::value<<endl;
  cout<<has_print<A>::value <<endl;
  cout<<has_print<TestClass> :: value<<endl;
  // check if a target_type is defined in class
  cout << experimental:: is_detected<has_typedef_target_type_t, TestClass>::value << endl;
TypeA atype(10);
TypeB btype(11);
  cout<<add1(atype).value <<endl;
//  cout<<add1(btype).value <<endl;

}

