#pragma once

#include <ctime>

#include "prometheus/metric_type.h"

namespace prometheus {

/// \brief The base metric.
///
class Metric {
 public:
  static const MetricType metric_type{MetricType::Untyped};

  Metric() = default;
  ~Metric() = default;

  bool Expired(time_t current_ts) const;

 protected:
  void UpdateTS();
  //
  // ClientMetric Collect() const;

 private:
  time_t last_update_ts_ = 0;
};

}  // namespace prometheus
