#pragma once

#include <stdint.h>

#include <chrono>
#include <ratio>
#include <type_traits>

// Exists only to give points in time their own type. It has no now(), so TimerLogic
// can never read a clock: the hardware layer makes each point in time. Leaving out now()
// bends the standard's Clock requirements, but time_point never calls it.
// Static_asserts in the timer logic tests check that the two types can't be mixed up.
struct DeviceClock {
  // Named explicitly: std::chrono::milliseconds only promises at least 45 bits.
  using rep = uint64_t;
  using period = std::milli;
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<DeviceClock>;
  static constexpr bool is_steady = true;
};

// A length of time.
using Duration = DeviceClock::duration;
// A point in time on the device's monotonic clock, counted from boot.
using TimePoint = DeviceClock::time_point;

static_assert(sizeof(Duration::rep) == 8 && std::is_unsigned<Duration::rep>::value,
              "Time is a 64-bit unsigned count");
static_assert(std::is_same<Duration::period, std::milli>::value, "Time is counted in milliseconds");

// All Interval durations live here.
constexpr Duration FOCUS_DURATION = std::chrono::minutes(25);
constexpr Duration BREAK_DURATION = std::chrono::minutes(5);

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
// current point in time.
class TimerLogic {
 public:
  void primaryPressed(TimePoint now);
  void nextPressed(TimePoint now);
  void tick(TimePoint now);
  Snapshot snapshot() const;

 private:
  RunState runState_ = RunState::Ready;
  // Zero until the Interval is first paused.
  Duration elapsedAtLastPause_ = Duration::zero();
  // Set on starting and on resuming; only meaningful while Running.
  TimePoint runningSince_;
  TimePoint now_;
};
