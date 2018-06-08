//
// Created by Vesper on 6/7/18.
//
#include <type_traits>
#include <iostream>
#include <utility>
#include <memory>
#include <string>
#include <queue>
using namespace std;

// The problem comes from elevator simulation command_streamer construction
// Some command has no parameters Stop/Start
// Some command has parameters Goto floor 8, Set Speed 0.5 with different parameters type
//
// The ideal case would be I put all commands in the same command queue.
// idea comes from this https://aherrmann.github.io/programming/2014/10/19/type-erasure-with-merged-concepts/


class Command {
public:

    // the 5 default here
    ~Command(){}

    template <class T>
    Command(T &data) : self_(make_shared<Holder<T> >(data)) { cout<<"& ctor"<<endl;}

    template <class T>
    Command(T &&data) : self_(make_shared<Holder<T> >(forward<T>(data))) { cout<<"&& ctor"<<endl;}

    template<class T>
    T &operator = (T const &rhs)  {
        self_ = make_shared<Holder<T> >(rhs);
        cout<<"& assign"<<endl;
        return *this;
    }

    template <class T>
    T &operator= (T const &&rhs)  {
        self_ = make_shared<Holder<T> >(rhs);
        cout<<"&& assign"<<endl;
        return *this;
    }

    void print()const {
        self_->print();
    }
    void do_something(int param) {
        self_->do_something(param);
    }

private:
    struct Concept {
        virtual ~Concept() = default;
        virtual void  print()const =0;
        virtual void do_something( int param)  =0;
    };
    template <class T>
    class Holder : public Concept {
    public:
        Holder(T data) : data_(data) {}

        virtual void print()const override  {
            data_.print();
        }
        void do_something(int  param) override {
            data_.do_something(param);
        }
    private:
        T data_;
    };

    std::shared_ptr<Concept> self_;
};


struct StartCommand {
    StartCommand(){}
    void construct(StartCommand &)  {}
    void print() const {cout<<"StartCommand "<<endl;}
    void do_something(bool now) {
        cout<<boolalpha<<"start now? "<< now <<endl;
    }
};

struct StopCommand {
    StopCommand() {}
    void construct(StopCommand &)  {}
    void print()const {cout<<"StopCommand "<<endl;}
    void do_something(bool now) {
        cout<<boolalpha<<"stop now? "<< now <<endl;
    }
};

struct GotoCommand {
    int floor;
    GotoCommand(int _floor):floor(_floor) {}
    void construct(GotoCommand &t)  {
        floor = t.floor;
    }
    void print() const {cout<<"GotoFloor "<<floor<< endl;}
    void do_something(int next_floor) {
        cout<<"next floor  "<< next_floor <<endl;
    }
};

struct SetRateCommand {
    float rate;
    SetRateCommand(float _rate):rate(_rate) {}
    void construct(SetRateCommand &t)  {
        rate = t.rate;
    }
    void print()const {cout<<"SetRate "<<rate<< endl;}
    void do_something(float next_rate) {
        cout<<"set rate  "<< next_rate <<endl;
    }
};

struct SetSpeedCommand {
    float speed;
    SetSpeedCommand(float _speed):speed(_speed ){}
    void construct(SetSpeedCommand &t)  {
        speed = t.speed;
    }
    void print()const {cout<<"SetSpeed "<<speed<< endl;}
    void do_something(float next_spped) {
        cout<<"set speed  "<< next_spped <<endl;
    }
};


int main() {

    queue<Command> command_streamer;
    command_streamer.emplace(StartCommand{});
    command_streamer.emplace(SetRateCommand{0.6});
    command_streamer.emplace(SetSpeedCommand{10});
    command_streamer.emplace(GotoCommand{20});
    command_streamer.emplace(StopCommand{});
    while(!command_streamer.empty()) {
        auto  &e = command_streamer.front();
        //do stuff with the element
        // a better idea will even be command has its comman handler  register before
        // so it invoke e.handle() -->data already binded.
        e.do_something(10);
        if (typeid(e) == typeid(StopCommand{}))
            cout<<"Stop Command" <<endl;
        command_streamer.front().print();
        command_streamer.pop();
    }

    return 0;
}