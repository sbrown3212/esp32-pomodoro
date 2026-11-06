#include "timer_logic.h"

void TimerLogic::primaryPressed(uint64_t nowMs) {
  nowMs_ = nowMs;
  if (runState_ == RunState::Ready) {
    runState_ = RunState::Running;
    startedAtMs_ = nowMs;
  } else if (runState_ == RunState::Running) {
    runState_ = RunState::Paused;
    elapsedBeforeRunMs_ += nowMs - startedAtMs_;
  }
}

void TimerLogic::nextPressed(uint64_t nowMs) {
  nowMs_ = nowMs;
}

void TimerLogic::tick(uint64_t nowMs) {
  nowMs_ = nowMs;
}

Snapshot TimerLogic::snapshot() const {
  const uint64_t elapsedMs =
      elapsedBeforeRunMs_ + (runState_ == RunState::Running ? nowMs_ - startedAtMs_ : 0);
  const uint64_t remainingMs = elapsedMs < FOCUS_DURATION_MS ? FOCUS_DURATION_MS - elapsedMs : 0;
  // Rounding up shows 25:00 at the start, and 00:01 until the planned duration is reached.
  const uint64_t secondsToShow = (remainingMs + 999) / 1000;
  return {IntervalType::Focus, runState_, static_cast<uint32_t>(secondsToShow)};
}
