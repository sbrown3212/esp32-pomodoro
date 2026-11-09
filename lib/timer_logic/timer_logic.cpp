#include "timer_logic.h"

void TimerLogic::primaryPressed(TimePoint now) {
  now_ = now;
  switch (runState_) {
    case RunState::Ready:
    case RunState::Paused:
      runState_ = RunState::Running;
      runningSince_ = now;
      break;
    case RunState::Running:
      runState_ = RunState::Paused;
      elapsedAtLastPause_ += now - runningSince_;
      break;
  }
}

void TimerLogic::nextPressed(TimePoint now) {
  now_ = now;
}

void TimerLogic::tick(TimePoint now) {
  now_ = now;
}

Snapshot TimerLogic::snapshot() const {
  const Duration elapsed =
      elapsedAtLastPause_ + (runState_ == RunState::Running ? now_ - runningSince_ : Duration::zero());
  const Duration remaining = elapsed < FOCUS_DURATION ? FOCUS_DURATION - elapsed : Duration::zero();
  // Rounding up shows 25:00 at the start, and 00:01 until the planned duration is reached.
  // Written out because std::chrono::ceil needs C++17, and the native test build is C++14.
  const Duration oneSecond = std::chrono::seconds(1);
  const uint64_t secondsToShow = (remaining + oneSecond - Duration(1)) / oneSecond;
  return {IntervalType::Focus, runState_, static_cast<uint32_t>(secondsToShow)};
}
