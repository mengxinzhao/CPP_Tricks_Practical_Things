#include <iostream>
#include <string>
#include <string_view>

enum class RecordItem { metrics, thermals, NotImplemented };

template <RecordItem T>
struct RecordTraits {
  static constexpr const char *type = "";
};

template <>
struct RecordTraits<RecordItem::metrics> {
  static constexpr const char *type = "/metrics";
};

template <>
struct RecordTraits<RecordItem::thermals> {
  static constexpr const char *type = "/thermals";
};

using RecordMetrics = RecordTraits<RecordItem::metrics>;
using RecordThermals = RecordTraits<RecordItem::thermals>;

template <typename RecordTraits>
class Recorder;
class MetricsRecord;

class SecretAgency {
  template <typename RecordTraits>
  friend class Recorder;
  static void getInfo() { std::cout << "hhhhhhh" << std::endl; }
};

template <typename RecordTraits>
class Recorder {
 public:
  void record() {
    std::string prefix = RecordTraits::type;
    std::cout << "prefix: " << prefix << std::endl;
    //   std::cout<<"gRecordCollector: "<<gRecordCollector<RecordTraits><<std::endl;
    //   gRecordCollector<RecordTraits> = std::string_view(std::string("/secondary") +
    //   std::string(RecordTraits::type)); std::cout<<"gRecordCollector:
    //   "<<gRecordCollector<RecordTraits><<std::endl;
    SecretAgency::getInfo();
  }
};

class MetricsRecord : public Recorder<RecordTraits<RecordItem::metrics>> {
 public:
  MetricsRecord() : Recorder<RecordTraits<RecordItem::metrics>>() {}
};

struct TopicRecorder {
  template <typename RecordTraits>
  static void record();
};
