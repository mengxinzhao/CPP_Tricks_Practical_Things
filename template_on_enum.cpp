#include <iostream>
enum PolicyType {Default, Greedy, NotImplemented};

template<PolicyType  T>class Policy {
public:
    Policy() = default;
   ~Policy() = default;
};
 
template<>class Policy<Greedy> {
    int value;
    int method;
    int goal; 
public:
    Policy(int _value, int _method, int _goal):value(_value),method(_method),goal(_goal){}
   ~Policy() {}
    int set() {
	std::cout <<"Policy: " << Greedy <<" "<< value << " " << method << " " << goal<< std::endl;
	return 0;
    }
}; 

template<>class Policy<Default> {
   int value;
   int goal;
   
public:
   Policy(int _value, int _goal): value(_value), goal(_goal) {}
   ~Policy() {}
   int set() {
     std::cout <<"Policy: " << Default <<" "<< value << " "  << goal<< std::endl;
     return 0;
   }
};


int main() {
   Policy<Greedy> p{4,4,6};
   p.set();
   Policy<Default> d{4,10};
   d.set();
   Policy<NotImplemented> n;
   n.set();
}
