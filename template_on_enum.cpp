#include <iostream>
enum class PolicyType {Default, Greedy, NotImplemented};

template<PolicyType T> struct priority {
    enum {val = 99};
};

template<> struct priority<PolicyType::Default> { 
    enum {val = 0};
};


template<PolicyType  T>class Policy {
public:
    Policy() = default;
   ~Policy() = default;
   const int _priority = priority<T>::val;
   int set(){
     //std::cout <<"Policy: " <<static_cast<int>(T) <<" "<<_priority<<  std::endl;
	return 0;
   }
};

static inline int add(int a, int b) {
    return a+b;
}
template<>class Policy<PolicyType::Greedy> {
    int value;
    int method;
    int goal; 
public:
    Policy():value(-1),method(-1), goal(add(10,20)){}
    Policy(int _value, int _method, int _goal):value(_value),method(_method),goal(_goal){}
   ~Policy() {}
    int set() {
	std::cout <<"Policy: " <<"PolicyType::Greedy "<< value << " " << method << " " << goal<< std::endl;
	return 0;
    }
}; 

template<>class Policy<PolicyType::Default> {
   int value;
   int goal;
   const int _priority = priority<PolicyType::Default>::val;
public:
   Policy(int _value, int _goal): value(_value), goal(_goal) {}
   ~Policy() {}
   int set() {
     std::cout <<"Policy: " << "PolicyType::Default" <<" "<< value << " "  << goal<< std::endl;
     std::cout<<"priority: " << _priority<<std::endl; 
     return 0;
   }
   
};

template<PolicyType T>
class TestClass {
public:
    TestClass(int _id, const Policy<T> &_policy):id(_id), policy(_policy) {}
    TestClass() = default;
    ~TestClass(){}
template <PolicyType TT>
    int set( Policy<TT> &&_policy) {
        _policy.set();
        return 0;
    }
    int id;
    Policy<T> policy;
};

struct Number_of_CPUs {
   enum {val = 48};
};

#if 0
//doesn't work here
class AnotherPolicy {
public:
PolicyType type;
int prior;
AnotherPolicy(const PolicyType _type, const int _prior):type(_type), prior(_prior) {};
explicit AnotherPolicy(const PolicyType _type):type(_type),prior(priority<_type>::val){};
};
#endif 

int main() {
   Policy<PolicyType::Greedy> p{4,4,6};
   p.set();
   Policy<PolicyType::Default> d{4,10};
   d.set();
   Policy<PolicyType::NotImplemented> n;
   n.set();

   PolicyType type = static_cast<PolicyType>(22);
   TestClass<PolicyType::Greedy> test1(34,Policy<PolicyType::Greedy>(4,5,6));
   test1.policy.set();
   TestClass<PolicyType::Greedy> test2;
   test2.set(Policy<PolicyType::Default>{6,12});
   //AnotherPolicy test3(PolicyType::Default,5);
   //AnotherPolicy test4(PolicyType::Greedy);
return 0;
}
