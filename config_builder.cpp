//
// Created by Vesper on 6/8/18.
//


#include <iostream>
#include <utility>
#include "config_builder.h"
#include <string>
#include <vector>
#include <iterator>
using namespace std;

// If for each product I have a pre-defined supporting features and I need to
// build a configuration structure efficiently.
// Looking at chained-builder pattern
//
//        config_name, support_A, support_B, support_C
#define DEFINE_FEATURE(NAME) \
struct Feature##NAME {\
static const bool value =true;\
void get() const {\
        cout<<"value: "<<boolalpha<<value<< endl;\
    }\
};\


DEFINE_FEATURE(A)
DEFINE_FEATURE(B)
DEFINE_FEATURE(C)
DEFINE_FEATURE(D)

#define DEFINE_CONFIG2(NAME, FeatureA)\
auto config_##NAME = Config::create().set_feature(FeatureA{})();

#define DEFINE_CONFIG3(NAME, FeatureA,FeatureB, FeatureC)\
auto config_##NAME = Config::create().set_feature(FeatureA{}).set_feature(FeatureB{}).set_feature(FeatureC{})();

// how do I achive the configuration such as
// #define DEFINE_CONFIG3(NAME, FeatureA,...FeatureN)\
// auto config_##NAME = Config::create().set_feature(FeatureA{})....set_feature(FeatureN{})();
int main() {

    DEFINE_CONFIG2(NewProduc2, FeatureA);
    auto [begin, end]=  config_NewProduc2->get_features();
    auto iterator = begin;
    while(iterator!= end) {
        cout<<typeid(*iterator).name()<<endl;
        iterator->get();
        iterator++;

    }
    DEFINE_CONFIG3(NewProduc3, FeatureA,FeatureB,FeatureC);
    tie(begin, end) =  config_NewProduc3->get_features();
    iterator = begin;
    while(iterator!= end) {
        cout<<typeid(*iterator).name()<<endl;
        iterator->get();
        iterator++;
        if (typeid(*iterator) == typeid(FeatureB))
            cout<<"Feature B supported"<<endl;

    }
}
