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
 
template<>class Policy<PolicyType::Greedy> {
    int value;
    int method;
    int goal; 
public:
    Policy(int _value, int _method, int _goal):value(_value),method(_method),goal(_goal){}
   ~Policy() {}
    int set() {
	//std::cout <<"Policy: " << PolicyType::Greedy <<" "<< value << " " << method << " " << goal<< std::endl;
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
     //std::cout <<"Policy: " << PolicyType::Default <<" "<< value << " "  << goal<< std::endl;
     std::cout<<"priority: " << _priority<<std::endl; 
     return 0;
   }
   
};

struct Number_of_CPUs {
   enum {val = 48;};
};
int main() {
   Policy<PolicyType::Greedy> p{4,4,6};
   p.set();
   Policy<PolicyType::Default> d{4,10};
   d.set();
   Policy<PolicyType::NotImplemented> n;
   n.set();

   PolicyType type = static_cast<PolicyType>(22);
return 0;
}
