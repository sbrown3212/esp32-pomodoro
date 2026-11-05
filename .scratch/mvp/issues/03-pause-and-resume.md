# 03: Pause and resume

**What to build:** Primary always starts or stops the clock. Pressing Primary while Running pauses the Interval, and the header shows `PAUSED`. Pressing it again resumes from exactly where it stopped. Spec: `.scratch/mvp/spec.md` (stories 3–6).

**Blocked by:** 02

**Status:** ready-for-agent

- [ ] Primary while Running → Paused; Primary while Paused → Running
- [ ] Elapsed time is preserved exactly across one or more pauses, whatever the pause length
- [ ] The time shown does not change while Paused
- [ ] Header shows `PAUSED` while Paused
- [ ] Tests cover pausing and resuming in a Focus (Break is covered once ticket 04 introduces it)
- [ ] Verified by hand on the device

## Comments

Ticket 02 added `test_primary_while_running_does_not_restart_the_countdown`, which pins Primary doing nothing while Running. Pausing replaces that behaviour, so rewrite the test as a pause test rather than keeping it passing.
