#include "prometheus/manual_summary.h"

namespace prometheus {

void ManualSummary::SetCountAndSum(std::uint64_t count, double sum) {
  std::lock_guard<std::mutex> lock(mutex_);
  count_ = count;
  sum_ = sum;
}

void ManualSummary::AddQuantile(double quantile, double value) {
  std::lock_guard<std::mutex> lock(mutex_);
  quantiles_.emplace(quantile, value);
  UpdateTS();
}

ClientMetric ManualSummary::Collect() const {
  auto metric = ClientMetric{};

  std::lock_guard<std::mutex> lock(mutex_);

  metric.summary.quantile.reserve(quantiles_.size());
  for (const auto& quantile : quantiles_) {
    auto metricQuantile = ClientMetric::Quantile{};
    metricQuantile.quantile = quantile.first;
    metricQuantile.value = quantile.second;
    metric.summary.quantile.push_back(std::move(metricQuantile));
  }
  metric.summary.sample_count = count_;
  metric.summary.sample_sum = sum_;

  return metric;
}

}  // namespace prometheus
