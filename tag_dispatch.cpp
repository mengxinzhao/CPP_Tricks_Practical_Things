#include <iostream>
#include <type_traits>
#include <memory>
using namespace std;

template< typename T> struct is_shared_ptr {
    static const bool value = false;
};

template< typename T >
struct is_shared_ptr<shared_ptr<T> > {
    static const bool value = true;
};

struct TestClass {
    int a=100;
};

typedef TestClass TestClass_t;

// these are through sfinae
template<typename T>
T* clone_instance(T &a) {
    T *new_instance = new T(a);
    return new_instance;
}

template<typename T>
shared_ptr<T> clone_instance(shared_ptr<T> &p) {
    shared_ptr<T> new_instance = p;
    return new_instance;
}

template< typename T>
typename enable_if<is_shared_ptr<T>::value, T >::type  clone_instance (T &v) {
    return clone_instance(v);
}

// through tag dispatch. a true type and false type
// if the function signature is rather the same,only the argument types are different 
// between different overloadings,tag dispatch seems to more effective
template<typename T>
bool check(T a, false_type) {
    return false;
}

template<typename T>
bool check(shared_ptr<T> p,true_type) {
    return true;
}

template< typename T>
bool check (T &v) {
    return check(v, conditional_t<is_shared_ptr<T>::value,true_type, false_type>{} );
}

//tag dispatch
struct Alg1{
    static const string name;
};
string const Alg1::name="Alg1";

struct Alg2{
    static const string name;
};
string const Alg2::name = "Alg2";

class CompareTest1 {
public:
    using AlgorithmClass = Alg1;
    int a = 100;
};

class CompareTest2 {
public:
    using AlgorithmClass = Alg2;
    int b = 100;
};

template <typename T>
bool is_equal (T t) {
    return is_equal(t,typename T::AlgorithmClass() );
}
template <typename T>
bool is_equal (T t, Alg1 ){
    cout<<"Using "<<Alg1::name<<endl;
    return true;
}

template <typename T>
bool is_equal (T t, Alg2 ){
    cout<<"Using "<<Alg2::name<<endl;
    return false;
}


int main (){
    cout<<is_shared_ptr<int>::value<<endl;
    typedef shared_ptr<int> shared_int_ptr;
    cout<<is_shared_ptr<shared_int_ptr>::value<<endl;
    TestClass_t test;
    auto test_ptr = make_shared<TestClass_t>(test);
    auto p = clone_instance(test);
    cout<<p->a<<endl;
    auto shared_p =clone_instance(test_ptr);
    cout<<"checking a shared p. expecting true..."<<boolalpha<<check(shared_p)<<endl;
    cout<<"checking a pointer. expecting false..."<<boolalpha<<check(p)<<endl;
    CompareTest1 test1;
    CompareTest2 test2;
    is_equal(test1);
    is_equal(test2);
    return 0;
}
