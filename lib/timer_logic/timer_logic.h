#pragma once

#include <stdint.h>

// All Interval durations live here.
constexpr uint64_t FOCUS_DURATION_MS = 25 * 60 * 1000;
constexpr uint64_t BREAK_DURATION_MS = 5 * 60 * 1000;

enum class IntervalType { Focus, Break };
enum class RunState { Ready, Running, Paused };

struct Snapshot {
  IntervalType intervalType;
  RunState runState;
  uint32_t secondsToShow;

  bool operator==(const Snapshot &other) const {
    return intervalType == other.intervalType && runState == other.runState &&
           secondsToShow == other.secondsToShow;
  }
  bool operator!=(const Snapshot &other) const { return !(*this == other); }
};

// Holds all timer behaviour. It never reads a clock: each input carries the
// current monotonic time as a 64-bit millisecond timestamp.
class TimerLogic {
 public:
  void primaryPressed(uint64_t nowMs);
  void nextPressed(uint64_t nowMs);
  void tick(uint64_t nowMs);
  Snapshot snapshot() const;

 private:
  RunState runState_ = RunState::Ready;
  // Zero until the Interval is first paused.
  uint64_t elapsedAtLastPauseMs_ = 0;
  // Set on starting and on resuming; only meaningful while Running.
  uint64_t runningSinceMs_ = 0;
  uint64_t nowMs_ = 0;
};
