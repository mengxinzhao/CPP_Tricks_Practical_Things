//
// Created by Vesper on 6/8/18.
//


#include <iostream>
#include <utility>
#include "config_builder.h"

using namespace std;

// If for each product I have a pre-defined supporting features and I need to
// build a configuration structure efficiently.
// Looking at chained-builder pattern
//
//        config_name, support_A, support_B, support_C
#define DEFINE_CONFIG(NAME, VAR_A, VAR_B, VAR_C)\
auto config_##NAME = Config::create().set_support_A(VAR_A).set_support_B(VAR_B).set_support_C(VAR_C)();

ConfigBuilder& ConfigBuilder:: set_support_A(bool value) {
    config->support_A = value;
    return *this;
}
ConfigBuilder& ConfigBuilder::set_support_B(bool value) {
    config->support_B = value;
    return *this;
}
ConfigBuilder& ConfigBuilder::set_support_C(bool value) {
    config->support_C = value;
    return *this;
}


int main() {
    DEFINE_CONFIG(NewProduct, true, true, false);
    DEFINE_CONFIG(OldProduct, true, false, false);

}
