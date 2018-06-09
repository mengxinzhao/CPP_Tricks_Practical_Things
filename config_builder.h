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
        return config;
    }
    template<typename T>
    ConfigBuilder& set_feature(T &t) {
        config->table.push_back(t);
        return *this;
    }
    template<typename T>
    ConfigBuilder& set_feature(T &&t) {
        config->table.emplace_back(move(t));
        return *this;
    }
};


#endif //CPP_TRICKS_PRACTICAL_THINGS_CONFIG_BUILDER_H
