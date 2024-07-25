#include "prometheus/counter.h"

namespace prometheus {

void Counter::Increment() { gauge_.Increment(); }

void Counter::Increment(const double val) {
  if (val < 0.0) {
    return;
  }
  gauge_.Increment(val);
}

double Counter::Value() const { return gauge_.Value(); }

void Counter::Reset() { gauge_.Set(0); }

ClientMetric Counter::Collect() const {
  if (collect_callback_ != nullptr) return collect_callback_();

  ClientMetric metric;
  metric.counter.value = Value();
  return metric;
}

void Counter::AddCollectCallback(CollectCallbackPtr cb) {
  collect_callback_ = std::move(cb);
}

void Counter::UpdateViaCallback() {
  if (!collect_callback_) return;

  auto res = collect_callback_();
  gauge_.Set(res.counter.value);
}

}  // namespace prometheus
