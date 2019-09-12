#include <iostream>
#include <string>

enum class RecordItem {metrics, thermals, NotImplemented};

template<RecordItem T>
struct RecordTraits
{
    static constexpr const char *type="";
};

template<>
struct RecordTraits<RecordItem::metrics>{
    static constexpr const char *type="/metrics";
};

template<>
struct RecordTraits<RecordItem::thermals>{
    static constexpr const char *type="/thermals";
};

template<typename RecordTraits>
struct Record
{
  void record(){
      std::string prefix = RecordTraits::type;
      std::cout<<prefix<<std::endl;
  }
};

int main() {
    Record<RecordTraits<RecordItem::metrics>> metrics_rd;
    Record<RecordTraits<RecordItem::thermals>> thermal_rd;
    metrics_rd.record();
    thermal_rd.record();
}



