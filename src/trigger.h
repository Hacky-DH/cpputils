#pragma once

#include <chrono>

namespace utils {

using namespace std::chrono;

// trigger at most once every N seconds or once every N counts.
class Trigger {
  size_t every_count_;
  size_t last_triggered_count_ = 0;
  seconds every_secs_;
  time_point<high_resolution_clock> last_triggered_time_;

  using float_seconds = std::chrono::duration<float>;

 public:
  Trigger(size_t every_secs, size_t every_count)
      : every_secs_(every_secs), every_count_(every_count) {
    Reset();
  }
  void Reset() {
    last_triggered_count_ = 0;
    last_triggered_time_ = {};
  }
  bool ShouldTrigger(size_t count) {
    if (last_triggered_count_ == 0) {
      return true;
    }
    if (last_triggered_count_ == count) {
      return false;
    }
    if (every_secs_.count() > 0 and
        high_resolution_clock::now() >= last_triggered_time_ + every_secs_) {
      return true;
    }
    if (every_count_ > 0 and count >= last_triggered_count_ + every_count_) {
      return true;
    }
    return false;
  }
  // return elapsed_secs is the number of seconds
  // between the current trigger and the last one.
  float Update(size_t count) {
    auto now = high_resolution_clock::now();
    if (last_triggered_time_.time_since_epoch().count() == 0) {
      last_triggered_time_ = now;
      last_triggered_count_ = count;
      return 0.0;
    }
    auto elapsed_time = now - last_triggered_time_;
    auto elapsed_secs = duration_cast<float_seconds>(elapsed_time).count();
    last_triggered_time_ = now;
    last_triggered_count_ = count;
    return elapsed_secs;
  }
};

}  // namespace utils
