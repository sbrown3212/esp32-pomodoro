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
  intervalType_ = intervalType_ == IntervalType::Focus ? IntervalType::Break : IntervalType::Focus;
  runState_ = RunState::Ready;
  elapsedAtLastPause_ = Duration::zero();
}

void TimerLogic::tick(TimePoint now) {
  now_ = now;
}

Snapshot TimerLogic::snapshot() const {
  const Duration elapsed =
      elapsedAtLastPause_ + (runState_ == RunState::Running ? now_ - runningSince_ : Duration::zero());
  const Duration planned = intervalType_ == IntervalType::Focus ? FOCUS_DURATION : BREAK_DURATION;
  const Duration remaining = elapsed < planned ? planned - elapsed : Duration::zero();
  // Rounding up shows 25:00 at the start, and 00:01 until the planned duration is reached.
  // Written out because std::chrono::ceil needs C++17, and the native test build is C++14.
  const Duration oneSecond = std::chrono::seconds(1);
  const uint64_t secondsToShow = (remaining + oneSecond - Duration(1)) / oneSecond;
  // Measured from the shown second, not the exact elapsed time, so the bar never moves between seconds.
  const uint64_t plannedSeconds = planned / oneSecond;
  const uint64_t progressThousandths = (plannedSeconds - secondsToShow) * 1000 / plannedSeconds;
  return {intervalType_, runState_, static_cast<uint32_t>(secondsToShow), static_cast<uint16_t>(progressThousandths)};
}
