//
// Created by Vesper on 6/8/18.
//

#ifndef CPP_TRICKS_PRACTICAL_THINGS_CONFIG_BUILDER_H
#define CPP_TRICKS_PRACTICAL_THINGS_CONFIG_BUILDER_H

#include <iostream>
#include <utility>
#include <memory>
#include "config.h"
using namespace std;

struct ConfigBuilder {
    shared_ptr<Config> config;
    ConfigBuilder() = delete;
    explicit  ConfigBuilder(shared_ptr<Config> c): config{c}{}

    // overloading operator()
    shared_ptr<Config>  operator ()()  const {
        return move(config);
    }

    ConfigBuilder& set_support_A(bool value) ;
    ConfigBuilder& set_support_B(bool value) ;
    ConfigBuilder& set_support_C(bool value) ;
};


#endif //CPP_TRICKS_PRACTICAL_THINGS_CONFIG_BUILDER_H
