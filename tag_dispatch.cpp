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
    return 0;
}
