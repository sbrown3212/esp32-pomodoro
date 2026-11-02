# 02: Tracer bullet: a Ready Focus that starts and counts down

**What to build:** The first end-to-end slice of real behaviour. The device powers on as a Ready Focus showing 25:00. Pressing Primary starts it Running and it counts down second by second on the screen. This introduces the timer logic module behind the single test seam described in the spec: *Primary pressed*, *Next pressed* and *tick* inputs, each carrying a 64-bit millisecond timestamp, and a snapshot output. The hardware layer becomes a thin translator: button presses and the ESP32's 64-bit clock in, snapshot rendered out. Spec: `.scratch/mvp/spec.md` (stories 1, 2, 15, 33–36, 38, 46, 48).

**Blocked by:** 01

**Status:** ready-for-agent

- [ ] Timer logic is pure C++ with no Arduino, display, GPIO or clock dependency, and is developed test-first in the native environment
- [ ] Snapshot exposes at least: Interval type, run state, time to show in whole seconds
- [ ] Power-on snapshot is Ready, Focus, 25:00
- [ ] Primary from Ready makes the Focus Running; subsequent ticks show the remaining time decreasing
- [ ] Elapsed time is computed from the timestamps, not by counting ticks: irregular or sparse tick spacing still yields the correct remaining time
- [ ] Timestamps are 64-bit milliseconds; the hardware layer takes them from the ESP32's 64-bit clock, not the 32-bit `millis()`; a test with large timestamps (weeks of uptime) gives correct times
- [ ] Displayed seconds start at 25:00 and round consistently
- [ ] Focus and Break durations are named constants in one place
- [ ] The hello-world screen is replaced by a header row (Interval type on the left, run state on the right) and large `mm:ss` digits
- [ ] The display is redrawn only when the snapshot changes; no visible flicker (checked by hand)
- [ ] Next is accepted by the interface but its behaviour is left to ticket 04
