# 09: Give points in time and lengths of time their own types

**What to build:** A refactor with no change in behaviour. Today the timer logic stores both points in time (`nowMs`, `runningSinceMs_`) and lengths of time (`FOCUS_DURATION_MS`, `elapsedAtLastPauseMs_`) as a plain `uint64_t`, so the compiler can't stop them being mixed up. Use `std::chrono` to give each its own type: a `time_point` on a small device clock type for points in time, and a `duration` for lengths of time. Subtracting two points in time then gives a length, adding two points in time fails to compile, and changing units has to be written out. This lands before Overtime (06), so the Overtime limit, the alert's flash window (07) and the blanking delay (08) all use the new types. Spec: `.scratch/mvp/spec.md` (Modules → Timer logic, Time); the spec is unchanged.

**Blocked by:** 03

**Status:** ready-for-agent

- [ ] Points in time and lengths of time have distinct types. Adding two points in time, or passing a length where a point in time is expected, fails to compile (check it with `static_assert`s or by hand, and say which in the comments)
- [ ] Both types hold a 64-bit millisecond count, named explicitly (e.g. `std::chrono::duration<uint64_t, std::milli>`), not left to `std::chrono::milliseconds`, whose size the standard doesn't fix; a `static_assert` pins this
- [ ] The device clock type has no `now()`, so the timer logic still never reads a clock; a one-line comment says why the type exists
- [ ] `FOCUS_DURATION_MS` and `BREAK_DURATION_MS` become lengths of time, still named constants in one place
- [ ] The inputs (Primary pressed, Next pressed, tick) take a point in time; the snapshot's time to show stays in whole seconds
- [ ] The hardware layer turns `esp_timer_get_time()` into a point in time in one place
- [ ] Behaviour is unchanged: existing tests change only in how they write timestamps, and no expected value changes
- [ ] Verified by hand: the device starts, counts down, pauses and resumes as before

## Comments

Doing this before 06 means one conversion now, instead of converting the Overtime limit, the alert's flash window and the blanking delay later. If 04 or 05 land first, convert any time arithmetic they add too.
