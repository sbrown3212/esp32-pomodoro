#include <unity.h>

#include <chrono>
#include <type_traits>
#include <utility>

#include "timer_logic.h"

void setUp() {}
void tearDown() {}

namespace {

// Compile-time checks that points in time and lengths of time can't be mixed up.
template <typename A, typename B, typename = void>
struct CanAdd : std::false_type {};
template <typename A, typename B>
struct CanAdd<A, B, decltype(void(std::declval<A>() + std::declval<B>()))> : std::true_type {};

static_assert(!CanAdd<TimePoint, TimePoint>::value, "adding two points in time must not compile");
static_assert(CanAdd<TimePoint, Duration>::value, "a point in time plus a length is a point in time");
static_assert(std::is_same<decltype(TimePoint() - TimePoint()), Duration>::value,
              "subtracting two points in time gives a length");
static_assert(!std::is_convertible<Duration, TimePoint>::value,
              "a length must not pass where a point in time is expected");
static_assert(!std::is_convertible<uint64_t, TimePoint>::value,
              "a bare number must not pass where a point in time is expected");

TimePoint atMs(uint64_t ms) {
  return TimePoint(Duration(ms));
}

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

  timer.primaryPressed(atMs(5000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 1500, timer.snapshot());

  timer.tick(atMs(6000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 1499, timer.snapshot());

  timer.tick(atMs(7000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 1498, timer.snapshot());
}

void test_shown_seconds_round_up_so_a_part_second_still_counts() {
  TimerLogic timer;
  timer.primaryPressed(atMs(0));

  timer.tick(atMs(1));
  TEST_ASSERT_EQUAL_UINT32(1500, timer.snapshot().secondsToShow);

  timer.tick(atMs(999));
  TEST_ASSERT_EQUAL_UINT32(1500, timer.snapshot().secondsToShow);

  timer.tick(atMs(1000));
  TEST_ASSERT_EQUAL_UINT32(1499, timer.snapshot().secondsToShow);

  timer.tick(atMs(1001));
  TEST_ASSERT_EQUAL_UINT32(1499, timer.snapshot().secondsToShow);

  // 24:59.999 into the Focus, one millisecond is left.
  timer.tick(atMs(1499999));
  TEST_ASSERT_EQUAL_UINT32(1, timer.snapshot().secondsToShow);
}

void test_irregular_and_sparse_ticks_still_give_the_correct_time() {
  TimerLogic timer;
  timer.primaryPressed(atMs(1000));

  timer.tick(atMs(1003));
  timer.tick(atMs(1700));
  timer.tick(atMs(61000));  // A whole minute with no ticks.
  TEST_ASSERT_EQUAL_UINT32(1440, timer.snapshot().secondsToShow);

  timer.tick(atMs(61001));
  timer.tick(atMs(601000));  // Nine more minutes from just one tick.
  TEST_ASSERT_EQUAL_UINT32(900, timer.snapshot().secondsToShow);
}

void test_correct_time_after_weeks_of_uptime() {
  TimerLogic timer;
  const TimePoint sixtyDays = TimePoint(std::chrono::hours(24 * 60));

  timer.primaryPressed(sixtyDays);
  timer.tick(sixtyDays + std::chrono::seconds(90));

  TEST_ASSERT_EQUAL_UINT32(1410, timer.snapshot().secondsToShow);
}

void test_correct_time_when_a_focus_spans_the_32_bit_millisecond_rollover() {
  TimerLogic timer;
  const TimePoint rollover = atMs(4294967296ULL);  // 2^32 ms: where a 32-bit count wraps

  timer.primaryPressed(rollover - std::chrono::seconds(10));
  timer.tick(rollover + std::chrono::seconds(50));

  TEST_ASSERT_EQUAL_UINT32(1440, timer.snapshot().secondsToShow);
}

// Until ticket 06 adds Overtime, the countdown stops at 00:00.
void test_countdown_stops_at_zero_at_and_after_the_planned_duration() {
  TimerLogic timer;
  timer.primaryPressed(atMs(0));

  timer.tick(atMs(1500000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 0, timer.snapshot());

  timer.tick(atMs(1500001));
  assertSnapshot(IntervalType::Focus, RunState::Running, 0, timer.snapshot());

  timer.tick(atMs(1510000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 0, timer.snapshot());
}

void test_primary_while_running_pauses_the_focus() {
  TimerLogic timer;
  timer.primaryPressed(atMs(0));
  timer.tick(atMs(10000));

  timer.primaryPressed(atMs(10000));

  assertSnapshot(IntervalType::Focus, RunState::Paused, 1490, timer.snapshot());
}

void test_time_shown_does_not_change_while_paused() {
  TimerLogic timer;
  timer.primaryPressed(atMs(0));
  timer.primaryPressed(atMs(10000));

  timer.tick(atMs(10001));
  assertSnapshot(IntervalType::Focus, RunState::Paused, 1490, timer.snapshot());

  timer.tick(atMs(70000));
  assertSnapshot(IntervalType::Focus, RunState::Paused, 1490, timer.snapshot());

  // Far longer than the Focus itself.
  timer.tick(atMs(10000000));
  assertSnapshot(IntervalType::Focus, RunState::Paused, 1490, timer.snapshot());
}

void test_primary_while_paused_resumes_from_where_it_stopped() {
  TimerLogic timer;
  timer.primaryPressed(atMs(0));
  timer.primaryPressed(atMs(10000));

  timer.primaryPressed(atMs(70000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 1490, timer.snapshot());

  timer.tick(atMs(75000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 1485, timer.snapshot());
}

void test_part_seconds_across_several_pauses_add_up_exactly() {
  TimerLogic timer;

  // Runs of 0.4 s, 0.7 s and 0.9 s, split by pauses of very different lengths.
  timer.primaryPressed(atMs(1000));
  timer.primaryPressed(atMs(1400));
  timer.primaryPressed(atMs(6400));
  timer.primaryPressed(atMs(7100));
  timer.primaryPressed(atMs(3607100));
  timer.tick(atMs(3608000));

  // 2.0 s elapsed in total.
  assertSnapshot(IntervalType::Focus, RunState::Running, 1498, timer.snapshot());
}

void test_snapshot_only_changes_when_the_shown_second_or_state_changes() {
  TimerLogic timer;
  const Snapshot ready = timer.snapshot();

  timer.tick(atMs(500));
  TEST_ASSERT_TRUE(ready == timer.snapshot());

  timer.primaryPressed(atMs(1000));
  const Snapshot started = timer.snapshot();
  TEST_ASSERT_TRUE(ready != started);

  timer.tick(atMs(1999));
  TEST_ASSERT_TRUE(started == timer.snapshot());

  timer.tick(atMs(2000));
  TEST_ASSERT_TRUE(started != timer.snapshot());
}

void test_next_from_a_ready_focus_gives_a_ready_break_at_5_00() {
  TimerLogic timer;

  timer.nextPressed(atMs(1000));

  assertSnapshot(IntervalType::Break, RunState::Ready, 300, timer.snapshot());
}

void test_next_from_a_running_focus_gives_a_ready_break_that_does_not_count_down() {
  TimerLogic timer;
  timer.primaryPressed(atMs(0));
  timer.tick(atMs(60000));

  timer.nextPressed(atMs(60000));
  assertSnapshot(IntervalType::Break, RunState::Ready, 300, timer.snapshot());

  timer.tick(atMs(120000));
  assertSnapshot(IntervalType::Break, RunState::Ready, 300, timer.snapshot());
}

void test_next_from_a_paused_focus_gives_a_break_at_its_full_duration() {
  TimerLogic timer;
  timer.primaryPressed(atMs(0));
  timer.primaryPressed(atMs(60000));

  timer.nextPressed(atMs(70000));
  assertSnapshot(IntervalType::Break, RunState::Ready, 300, timer.snapshot());

  // The Focus's minute must not carry into the Break.
  timer.primaryPressed(atMs(80000));
  timer.tick(atMs(90000));
  assertSnapshot(IntervalType::Break, RunState::Running, 290, timer.snapshot());
}

void test_next_from_a_ready_break_gives_back_a_ready_focus_at_25_00() {
  TimerLogic timer;
  timer.nextPressed(atMs(1000));

  timer.nextPressed(atMs(2000));

  assertSnapshot(IntervalType::Focus, RunState::Ready, 1500, timer.snapshot());
}

void test_primary_starts_a_break_that_counts_down_to_zero_at_5_00() {
  TimerLogic timer;
  timer.nextPressed(atMs(0));

  timer.primaryPressed(atMs(1000));
  assertSnapshot(IntervalType::Break, RunState::Running, 300, timer.snapshot());

  timer.tick(atMs(2000));
  assertSnapshot(IntervalType::Break, RunState::Running, 299, timer.snapshot());

  // 4:59.999 into the Break, one millisecond is left.
  timer.tick(atMs(300999));
  assertSnapshot(IntervalType::Break, RunState::Running, 1, timer.snapshot());

  timer.tick(atMs(301000));
  assertSnapshot(IntervalType::Break, RunState::Running, 0, timer.snapshot());
}

void test_primary_pauses_and_resumes_a_break_from_where_it_stopped() {
  TimerLogic timer;
  timer.nextPressed(atMs(0));
  timer.primaryPressed(atMs(0));

  timer.primaryPressed(atMs(10000));
  assertSnapshot(IntervalType::Break, RunState::Paused, 290, timer.snapshot());

  timer.tick(atMs(70000));
  assertSnapshot(IntervalType::Break, RunState::Paused, 290, timer.snapshot());

  timer.primaryPressed(atMs(70000));
  assertSnapshot(IntervalType::Break, RunState::Running, 290, timer.snapshot());

  timer.tick(atMs(75000));
  assertSnapshot(IntervalType::Break, RunState::Running, 285, timer.snapshot());
}

void test_next_from_a_running_break_gives_a_ready_focus_that_does_not_count_down() {
  TimerLogic timer;
  timer.nextPressed(atMs(0));
  timer.primaryPressed(atMs(0));
  timer.tick(atMs(60000));

  timer.nextPressed(atMs(60000));
  assertSnapshot(IntervalType::Focus, RunState::Ready, 1500, timer.snapshot());

  timer.tick(atMs(120000));
  assertSnapshot(IntervalType::Focus, RunState::Ready, 1500, timer.snapshot());
}

void test_next_from_a_paused_break_gives_a_focus_at_its_full_duration() {
  TimerLogic timer;
  timer.nextPressed(atMs(0));
  timer.primaryPressed(atMs(0));
  timer.primaryPressed(atMs(60000));

  timer.nextPressed(atMs(70000));
  assertSnapshot(IntervalType::Focus, RunState::Ready, 1500, timer.snapshot());

  // The Break's minute must not carry into the Focus.
  timer.primaryPressed(atMs(80000));
  timer.tick(atMs(90000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 1490, timer.snapshot());
}

void test_next_twice_from_a_running_focus_gives_a_fresh_ready_focus() {
  TimerLogic timer;
  timer.primaryPressed(atMs(0));
  timer.tick(atMs(600000));

  timer.nextPressed(atMs(600000));
  timer.nextPressed(atMs(601000));

  assertSnapshot(IntervalType::Focus, RunState::Ready, 1500, timer.snapshot());
}

void test_next_twice_from_a_paused_focus_gives_a_fresh_ready_focus() {
  TimerLogic timer;
  timer.primaryPressed(atMs(0));
  timer.primaryPressed(atMs(600000));

  timer.nextPressed(atMs(700000));
  timer.nextPressed(atMs(701000));
  assertSnapshot(IntervalType::Focus, RunState::Ready, 1500, timer.snapshot());

  timer.primaryPressed(atMs(800000));
  timer.tick(atMs(801000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 1499, timer.snapshot());
}

void test_a_full_focus_break_focus_loop_alternates_types() {
  TimerLogic timer;

  timer.primaryPressed(atMs(0));
  timer.tick(atMs(1500000));
  timer.nextPressed(atMs(1500000));
  assertSnapshot(IntervalType::Break, RunState::Ready, 300, timer.snapshot());

  timer.primaryPressed(atMs(1510000));
  timer.tick(atMs(1810000));
  timer.nextPressed(atMs(1810000));
  assertSnapshot(IntervalType::Focus, RunState::Ready, 1500, timer.snapshot());

  timer.primaryPressed(atMs(1820000));
  timer.tick(atMs(1830000));
  assertSnapshot(IntervalType::Focus, RunState::Running, 1490, timer.snapshot());

  timer.nextPressed(atMs(1830000));
  assertSnapshot(IntervalType::Break, RunState::Ready, 300, timer.snapshot());
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
  RUN_TEST(test_next_from_a_ready_focus_gives_a_ready_break_at_5_00);
  RUN_TEST(test_next_from_a_running_focus_gives_a_ready_break_that_does_not_count_down);
  RUN_TEST(test_next_from_a_paused_focus_gives_a_break_at_its_full_duration);
  RUN_TEST(test_next_from_a_ready_break_gives_back_a_ready_focus_at_25_00);
  RUN_TEST(test_primary_starts_a_break_that_counts_down_to_zero_at_5_00);
  RUN_TEST(test_primary_pauses_and_resumes_a_break_from_where_it_stopped);
  RUN_TEST(test_next_from_a_running_break_gives_a_ready_focus_that_does_not_count_down);
  RUN_TEST(test_next_from_a_paused_break_gives_a_focus_at_its_full_duration);
  RUN_TEST(test_next_twice_from_a_running_focus_gives_a_fresh_ready_focus);
  RUN_TEST(test_next_twice_from_a_paused_focus_gives_a_fresh_ready_focus);
  RUN_TEST(test_a_full_focus_break_focus_loop_alternates_types);
  return UNITY_END();
}
