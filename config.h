//
// Created by Vesper on 6/8/18.
//

#ifndef CPP_TRICKS_PRACTICAL_THINGS_CONFIG_H
#define CPP_TRICKS_PRACTICAL_THINGS_CONFIG_H
#include <iostream>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std;
struct ConfigBuilder;

namespace detail {
class Feature {
 public:
  // the 5 default here
  ~Feature() {}

  template <class T>
  Feature(T &data) : self_(make_shared<Holder<T> >(data)) {
    cout << "& ctor" << endl;
  }

  template <class T>
  Feature(T &&data) : self_(make_shared<Holder<T> >(forward<T>(data))) {
    cout << "&& ctor" << endl;
  }

  template <class T>
  T &operator=(T const &rhs) {
    if (this == &rhs) return *this;
    self_ = make_shared<Holder<T> >(rhs);
    cout << "& assign" << endl;
    return *this;
  }

  template <class T>
  T &operator=(T const &&rhs) {
    if (this == &rhs) return *this;
    self_ = make_shared<Holder<T> >(rhs);
    cout << "&& assign" << endl;
    return *this;
  }

  void get() const { self_->get(); }

  const type_info &type() const { return self_->type(); }

  // get the underlying data at the run time
  template <typename T>
  T get() {
    return dynamic_cast<Holder<T> &>(*self_).get_data();
  }

  template <typename T>
  const T get() const {
    return dynamic_cast<Holder<T> &>(*self_).get_data();
  }

 private:
  struct Concept {
    virtual ~Concept() = default;
    virtual void get() const = 0;
    virtual const type_info &type() const = 0;
  };

  template <class T>
  class Holder : public Concept {
   public:
    Holder(T data) : data_(data) {}
    void get() const override { data_.get(); }
    const type_info &type() const override { return typeid(T); }
    T get_data() const { return data_; }

   private:
    T data_;
  };

  shared_ptr<Concept> self_;
};

};  // namespace detail

class Config {
 public:
  ~Config() {}
  Config() {}
  Config(Config &rhs) = delete;
  Config &operator=(Config &rhs) = delete;
  Config(Config &&rhs) { move(rhs.table.begin(), rhs.table.end(), table.begin()); }

  friend class ConfigBuilder;
  static ConfigBuilder create();
  // return begin and end of the feature table
  typedef vector<detail::Feature>::const_iterator Iterator;
  const pair<Iterator, Iterator> get_features() { return {table.cbegin(), table.cend()}; }

 protected:
  vector<detail::Feature> table;
};

#endif  // CPP_TRICKS_PRACTICAL_THINGS_CONFIG_H
