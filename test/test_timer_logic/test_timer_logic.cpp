#include <unity.h>

#include "timer_logic.h"

void setUp() {}
void tearDown() {}

namespace {

void assertSnapshot(IntervalType type, RunState state, uint32_t seconds, const Snapshot &actual) {
  TEST_ASSERT_EQUAL_INT(static_cast<int>(type), static_cast<int>(actual.intervalType));
  TEST_ASSERT_EQUAL_INT(static_cast<int>(state), static_cast<int>(actual.runState));
  TEST_ASSERT_EQUAL_UINT32(seconds, actual.secondsToShow);
}

}  // namespace

void test_powers_on_as_a_ready_focus_at_25_00() {
  TimerLogic timer;

  assertSnapshot(IntervalType::Focus, RunState::Ready, 1500, timer.snapshot());
}

void test_primary_starts_the_focus_and_ticks_count_down() {
  TimerLogic timer;

  timer.primaryPressed(5000);
  assertSnapshot(IntervalType::Focus, RunState::Running, 1500, timer.snapshot());

  timer.tick(6000);
  assertSnapshot(IntervalType::Focus, RunState::Running, 1499, timer.snapshot());

  timer.tick(7000);
  assertSnapshot(IntervalType::Focus, RunState::Running, 1498, timer.snapshot());
}

void test_shown_seconds_round_up_so_a_part_second_still_counts() {
  TimerLogic timer;
  timer.primaryPressed(0);

  timer.tick(1);
  TEST_ASSERT_EQUAL_UINT32(1500, timer.snapshot().secondsToShow);

  timer.tick(999);
  TEST_ASSERT_EQUAL_UINT32(1500, timer.snapshot().secondsToShow);

  timer.tick(1000);
  TEST_ASSERT_EQUAL_UINT32(1499, timer.snapshot().secondsToShow);

  timer.tick(1001);
  TEST_ASSERT_EQUAL_UINT32(1499, timer.snapshot().secondsToShow);

  // 24:59.999 into the Focus, one millisecond is left.
  timer.tick(1499999);
  TEST_ASSERT_EQUAL_UINT32(1, timer.snapshot().secondsToShow);
}

void test_irregular_and_sparse_ticks_still_give_the_correct_time() {
  TimerLogic timer;
  timer.primaryPressed(1000);

  timer.tick(1003);
  timer.tick(1700);
  timer.tick(61000);  // A whole minute with no ticks.
  TEST_ASSERT_EQUAL_UINT32(1440, timer.snapshot().secondsToShow);

  timer.tick(61001);
  timer.tick(601000);  // Nine more minutes from just one tick.
  TEST_ASSERT_EQUAL_UINT32(900, timer.snapshot().secondsToShow);
}

void test_correct_time_after_weeks_of_uptime() {
  TimerLogic timer;
  const uint64_t sixtyDaysMs = 5184000000ULL;

  timer.primaryPressed(sixtyDaysMs);
  timer.tick(sixtyDaysMs + 90000);

  TEST_ASSERT_EQUAL_UINT32(1410, timer.snapshot().secondsToShow);
}

void test_correct_time_when_a_focus_spans_the_32_bit_millisecond_rollover() {
  TimerLogic timer;
  const uint64_t rolloverMs = 4294967296ULL;

  timer.primaryPressed(rolloverMs - 10000);
  timer.tick(rolloverMs + 50000);

  TEST_ASSERT_EQUAL_UINT32(1440, timer.snapshot().secondsToShow);
}

// Until ticket 06 adds Overtime, the countdown stops at 00:00.
void test_countdown_stops_at_zero_at_and_after_the_planned_duration() {
  TimerLogic timer;
  timer.primaryPressed(0);

  timer.tick(1500000);
  assertSnapshot(IntervalType::Focus, RunState::Running, 0, timer.snapshot());

  timer.tick(1500001);
  assertSnapshot(IntervalType::Focus, RunState::Running, 0, timer.snapshot());

  timer.tick(1510000);
  assertSnapshot(IntervalType::Focus, RunState::Running, 0, timer.snapshot());
}

void test_primary_while_running_pauses_the_focus() {
  TimerLogic timer;
  timer.primaryPressed(0);
  timer.tick(10000);

  timer.primaryPressed(10000);

  assertSnapshot(IntervalType::Focus, RunState::Paused, 1490, timer.snapshot());
}

void test_time_shown_does_not_change_while_paused() {
  TimerLogic timer;
  timer.primaryPressed(0);
  timer.primaryPressed(10000);

  timer.tick(10001);
  assertSnapshot(IntervalType::Focus, RunState::Paused, 1490, timer.snapshot());

  timer.tick(70000);
  assertSnapshot(IntervalType::Focus, RunState::Paused, 1490, timer.snapshot());

  // Far longer than the Focus itself.
  timer.tick(10000000);
  assertSnapshot(IntervalType::Focus, RunState::Paused, 1490, timer.snapshot());
}

void test_primary_while_paused_resumes_from_where_it_stopped() {
  TimerLogic timer;
  timer.primaryPressed(0);
  timer.primaryPressed(10000);

  timer.primaryPressed(70000);
  assertSnapshot(IntervalType::Focus, RunState::Running, 1490, timer.snapshot());

  timer.tick(75000);
  assertSnapshot(IntervalType::Focus, RunState::Running, 1485, timer.snapshot());
}

void test_part_seconds_across_several_pauses_add_up_exactly() {
  TimerLogic timer;

  // Runs of 0.4 s, 0.7 s and 0.9 s, split by pauses of very different lengths.
  timer.primaryPressed(1000);
  timer.primaryPressed(1400);
  timer.primaryPressed(6400);
  timer.primaryPressed(7100);
  timer.primaryPressed(3607100);
  timer.tick(3608000);

  // 2.0 s elapsed in total.
  assertSnapshot(IntervalType::Focus, RunState::Running, 1498, timer.snapshot());

  timer.primaryPressed(3608000);
  timer.tick(3700000);
  assertSnapshot(IntervalType::Focus, RunState::Paused, 1498, timer.snapshot());
}

void test_snapshot_only_changes_when_the_shown_second_or_state_changes() {
  TimerLogic timer;
  const Snapshot ready = timer.snapshot();

  timer.tick(500);
  TEST_ASSERT_TRUE(ready == timer.snapshot());

  timer.primaryPressed(1000);
  const Snapshot started = timer.snapshot();
  TEST_ASSERT_TRUE(ready != started);

  timer.tick(1999);
  TEST_ASSERT_TRUE(started == timer.snapshot());

  timer.tick(2000);
  TEST_ASSERT_TRUE(started != timer.snapshot());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_powers_on_as_a_ready_focus_at_25_00);
  RUN_TEST(test_primary_starts_the_focus_and_ticks_count_down);
  RUN_TEST(test_shown_seconds_round_up_so_a_part_second_still_counts);
  RUN_TEST(test_irregular_and_sparse_ticks_still_give_the_correct_time);
  RUN_TEST(test_correct_time_after_weeks_of_uptime);
  RUN_TEST(test_correct_time_when_a_focus_spans_the_32_bit_millisecond_rollover);
  RUN_TEST(test_countdown_stops_at_zero_at_and_after_the_planned_duration);
  RUN_TEST(test_primary_while_running_pauses_the_focus);
  RUN_TEST(test_time_shown_does_not_change_while_paused);
  RUN_TEST(test_primary_while_paused_resumes_from_where_it_stopped);
  RUN_TEST(test_part_seconds_across_several_pauses_add_up_exactly);
  RUN_TEST(test_snapshot_only_changes_when_the_shown_second_or_state_changes);
  return UNITY_END();
}
