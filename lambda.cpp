//
// Created by Vesper on 7/16/18.
//

// basic syntax
//[capture-list](parameter-list) -> return_type {body};
//// creates the type:
//struct __anonymous {
//    capture-members
//    __anonymous(capture-list) : capture-init { }
//
//    // Note the const on the operator below,
//    // it forbids modifying the actual members:
//    auto operator()(parameter-list) const -> return_type {
//        body
//    }
//};
// and an object:
//__anonymous{/* capture expressions */};

// generic lambdas are just templates under the hood
// [](auto ...p) {}
// auto and parameter packs are the same as they in templates


#include <iostream>
#include <utility>
#include <type_traits>
#include <algorithm>
#include <array>
#include <iomanip>

int g=10;
auto kitten = [] (){return g+1;};
auto cat = [g=g]() {return g+1;};


struct TestObj {
    int a{0};
    char c{'A'};
    double d{-1.5};
    bool is_all_bigger_than_0() const {return a >0 && (int) c >0 && d > 0;}

};

int main(){
    g = 20;
    std::cout<<kitten() <<" " << cat()<<std::endl;


    // generic lambda function
    std::array<int,3> a=  {1,2,4};
    std::array<double,3>b= {11.0, 22.0, 32.5};

    int i_total= 5;
    double d_total = 3.7;
    auto f = [&i_total, &d_total] (const auto v) {
        if constexpr (std::is_same_v<double, std::decay_t<decltype(v)>> ){
        //if constexpr (std::is_same_v<double,decltype(v)> ){
            // if no const auto v, then no need to use decay_t
            d_total+=v;
        }else
            i_total+=v;
        return;
    };
    std::for_each(std::begin(a), std::end(a),f);
    std::for_each(std::begin(b), std::end(b),f);
    std::cout<<i_total << " " <<std::setprecision(4)<< d_total<<std::endl;

    // by-move capture
    auto obj = std::make_unique<TestObj>();
    auto f2 = [p_obj = std::move(obj)]() {
        if (!p_obj->is_all_bigger_than_0()){
            p_obj->a++;
        }
        return p_obj->a;
    };
    std::cout <<std::boolalpha<< f2()<<std::endl;
    return 0;
}
