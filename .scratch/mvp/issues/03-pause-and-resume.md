# 03: Pause and resume

**What to build:** Primary always starts or stops the clock. Pressing Primary while Running pauses the Interval, and the header shows `PAUSED`. Pressing it again resumes from exactly where it stopped. Spec: `.scratch/mvp/spec.md` (stories 3–6).

**Blocked by:** 02

**Status:** needs-device-check

- [x] Primary while Running → Paused; Primary while Paused → Running
- [x] Elapsed time is preserved exactly across one or more pauses, whatever the pause length
- [x] The time shown does not change while Paused
- [x] Header shows `PAUSED` while Paused
- [x] Tests cover pausing and resuming in a Focus (Break is covered once ticket 04 introduces it)
- [ ] Verified by hand on the device

## Comments

Ticket 02 added `test_primary_while_running_does_not_restart_the_countdown`, which pins Primary doing nothing while Running. Pausing replaces that behaviour, so rewrite the test as a pause test rather than keeping it passing.

The placeholder test is now `test_primary_while_running_pauses_the_focus`. Elapsed time is banked in whole milliseconds on each pause, so part-seconds from separate runs add up exactly (tested with runs of 0.4 s, 0.7 s and 0.9 s split by pauses of 5 s and 1 hour). The hardware layer already rendered `PAUSED` for the Paused state, so `src/main.cpp` is unchanged. The firmware builds, but it has not run on a board.

Please check on the device: press Primary to pause, see `PAUSED` and the time hold still for a while, then press Primary again and see it carry on from the same second.
