//
// Created by Vesper on 6/8/18.
//

#ifndef CPP_TRICKS_PRACTICAL_THINGS_CONFIG_H
#define CPP_TRICKS_PRACTICAL_THINGS_CONFIG_H
#include <utility>
#include <memory>
#include <vector>
#include <iostream>
#include <utility>

using namespace std;
struct ConfigBuilder;

namespace detail {
    class Feature {
    public:

        // the 5 default here
        ~Feature() {}

        template<class T>
        Feature(T &data) : self_(make_shared<Holder < T> > (data)) { cout << "& ctor" << endl; }

        template<class T>
        Feature(T &&data) : self_(make_shared<Holder < T> > (forward<T>(data))) { cout << "&& ctor" << endl; }

        template<class T>
        T &operator=(T const &rhs) {
            if (this == &rhs)
                return *this;
            self_ = make_shared<Holder < T> > (rhs);
            cout << "& assign" << endl;
            return *this;
        }

        template<class T>
        T &operator=(T const &&rhs) {
            if (this == &rhs)
                return *this;
            self_ = make_shared<Holder < T> > (rhs);
            cout << "&& assign" << endl;
            return *this;
        }

        void get() const {
            self_->get();
        }


    private:
        struct Concept {
            virtual ~Concept() = default;
            virtual void get() const  = 0;
        };

        template<class T>
        class Holder : public Concept {
        public:
            Holder(T data) : data_(data) {}
            void get() const override  {
                data_.get();
            }

        private:
            T data_;
        };

        std::shared_ptr<Concept> self_;
    };

};

class Config
{
public:

    ~Config(){}
    Config() {}
    Config(Config &rhs)= delete;
    Config &operator = (Config &rhs) = delete;
    Config(Config &&rhs){
        move(rhs.table.begin(), rhs.table.end(), table.begin());
    }

    friend class ConfigBuilder;
    static ConfigBuilder create();
    // return begin and end of the feature table
    typedef vector<detail::Feature>::const_iterator Iterator;
    const pair<Iterator, Iterator > get_features()  {
        return {table.cbegin(),table.cend()};
    }
protected:

    vector<detail::Feature> table;




};

#endif //CPP_TRICKS_PRACTICAL_THINGS_CONFIG_H
