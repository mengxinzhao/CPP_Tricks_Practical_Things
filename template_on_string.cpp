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


template <typename RecordTraits>
std::string gRecordCollector = std::string(RecordTraits::type);

template<typename RecordTraits>
struct Recorder
{
  void record(){
      std::string prefix = RecordTraits::type;
      std::cout<<"prefix: "<<prefix<<std::endl;
      std::cout<<"gRecordCollector: "<<gRecordCollector<RecordTraits><<std::endl;
      gRecordCollector<RecordTraits> = std::string_view(std::string("/secondary") + std::string(RecordTraits::type));
      std::cout<<"gRecordCollector: "<<gRecordCollector<RecordTraits><<std::endl;
  }
};


int main() {
    Recorder<RecordTraits<RecordItem::metrics>> metrics_rd;
    Recorder<RecordTraits<RecordItem::thermals>> thermal_rd;
    metrics_rd.record();
    thermal_rd.record();
    std::cout<<gRecordCollector<RecordTraits<RecordItem::metrics>><<std::endl;
    std::cout<<gRecordCollector<RecordTraits<RecordItem::thermals>><<std::endl;
}



