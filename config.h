//
// Created by gltest on 6/8/18.
//

#ifndef CPP_TRICKS_PRACTICAL_THINGS_CONFIG_H
#define CPP_TRICKS_PRACTICAL_THINGS_CONFIG_H
#include <utility>
#include <memory>

using namespace std;
struct ConfigBuilder;
struct Config
{
    bool support_A;
    bool support_B;
    bool support_C;

    ~Config(){}
    Config():support_A(false),support_B(false),support_C(false){}
    Config(bool _support_A, bool _support_B,bool _support_C):support_A(_support_A), support_B(_support_B),support_C(_support_C){}
    Config(Config &rhs):support_A(rhs.support_A), support_B(rhs.support_B), support_C(rhs.support_C) {}
    Config(Config &&rhs) : support_A(move(rhs.support_A)),
                           support_B(move(rhs.support_B)),
                           support_C(move(rhs.support_C)) {}

    Config &operator = (Config &rhs){
        if (this == &rhs)
            return *this;
        support_A = rhs.support_A;
        support_B = rhs.support_B;
        support_C = rhs.support_C;
        return *this;
    }
    Config &operator = (Config &&rhs) {
        if (this == &rhs)
            return *this;
        support_A = move(rhs.support_A);
        support_B = move(rhs.support_B);
        support_C = move(rhs.support_C);
        return *this;
    }

    static ConfigBuilder create();

};

#endif //CPP_TRICKS_PRACTICAL_THINGS_CONFIG_H
