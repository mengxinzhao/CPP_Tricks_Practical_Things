//
// Created by Vesper on 6/11/18.
//

// codes from https://www.youtube.com/watch?v=R1G3P5SRXCw&t=1179s
// one template at the time of instantiation, can only look up the templates
// that are defined before it. It doesn't look up the all definition.
// So need to make sure the special cases are all defined before
#include <iostream>
template <typename T>
void P(T x) {
    std::cout << x << ' ';
}

void foo (char a) {P(3);P(a);}

template <typename ...Args>
void foo(int a, Args...args) {
    foo(args...);
    P(a);
}

template<typename ...Args>
void foo(char a, Args...args) {
    P(a);
    foo(args...);
}

int main(){
    foo('1','2',48,'4','5');
    // output
    // 1 2 3 5 52 48

    // call sequence P('1')-->P('2')-->P('4','5') --> P(48)
    // P('4','5') --> P((int)'5') -- > P((int)'4')
    // P((int)'5')--> P(3),P(5(ascii))
    // P((int)'4') -->  P(4(ascii))
}