#include "template_on_string.h"

template <typename RecordTraits>
std::string gRecordCollector = std::string(RecordTraits::type);

template <typename RecordTraits>
void TopicRecorder::record() {
  std::string prefix = RecordTraits::type;
  std::cout << "###prefix: " << prefix << std::endl;
  std::cout << "gRecordCollector: " << gRecordCollector<RecordTraits> << std::endl;
  gRecordCollector<RecordTraits> =
      std::string_view(std::string("/secondary") + std::string(RecordTraits::type));
  std::cout << "gRecordCollector: " << gRecordCollector<RecordTraits> << std::endl;
}

int main() {
  Recorder<RecordTraits<RecordItem::metrics>> metrics_rd;
  Recorder<RecordTraits<RecordItem::thermals>> thermal_rd;
  metrics_rd.record();
  thermal_rd.record();
  std::cout << gRecordCollector<RecordTraits<RecordItem::metrics>> << std::endl;
  std::cout << gRecordCollector<RecordTraits<RecordItem::thermals>> << std::endl;

  TopicRecorder rd;
  TopicRecorder::record<RecordMetrics>();
  MetricsRecord mrd;
  mrd.record();
}
