# 02: Tracer bullet: a Ready Focus that starts and counts down

**What to build:** The first end-to-end slice of real behaviour. The device powers on as a Ready Focus showing 25:00. Pressing Primary starts it Running and it counts down second by second on the screen. This introduces the timer logic module behind the single test seam described in the spec: *Primary pressed*, *Next pressed* and *tick* inputs, each carrying a 64-bit millisecond timestamp, and a snapshot output. The hardware layer becomes a thin translator: button presses and the ESP32's 64-bit clock in, snapshot rendered out. Spec: `.scratch/mvp/spec.md` (stories 1, 2, 15, 33–36, 38, 46, 48).

**Blocked by:** 01

**Status:** resolved

- [x] Timer logic is pure C++ with no Arduino, display, GPIO or clock dependency, and is developed test-first in the native environment
- [x] Snapshot exposes at least: Interval type, run state, time to show in whole seconds
- [x] Power-on snapshot is Ready, Focus, 25:00
- [x] Primary from Ready makes the Focus Running; subsequent ticks show the remaining time decreasing
- [x] Elapsed time is computed from the timestamps, not by counting ticks: irregular or sparse tick spacing still yields the correct remaining time
- [x] Timestamps are 64-bit milliseconds; the hardware layer takes them from the ESP32's 64-bit clock, not the 32-bit `millis()`; a test with large timestamps (weeks of uptime) gives correct times
- [x] Displayed seconds start at 25:00 and round consistently
- [x] Focus and Break durations are named constants in one place
- [x] The hello-world screen is replaced by a header row (Interval type on the left, run state on the right) and large `mm:ss` digits
- [x] The layout leaves a margin of a couple of pixels on every side (room for future pixel shifting)
- [x] Display contrast is set to a reduced level via a named constant, tuned by eye for comfortable reading at desk distance
- [x] The display is redrawn only when the snapshot changes; no visible flicker (checked by hand)
- [x] Next is accepted by the interface but its behaviour is left to ticket 04

## Comments

Timer logic is done and tested, and the firmware builds, but it has not run on hardware: no board was attached. The unticked items need checking on the device.

- Layout: rendered on the host with U8g2's own drawing code, which gave exactly 2 px of margin on the left, right and top, with rows 48-61 free for the progress bar in ticket 05. Please confirm it on the screen.
- Digit font: `logisoso28` is the largest candidate measured whose digits all have the same width, so the centred time doesn't shift as it counts. `logisoso32` is bigger, but its `1` is 2 px wider. Please check it reads well from across the desk.
- Contrast: `DISPLAY_CONTRAST` is 64, an untuned starting value (U8g2's default is 207). Please tune it by eye.
- Flicker: please watch the countdown. It redraws only when the snapshot changes, about once a second, and buttons aren't read during a redraw.
- Past 25:00 the countdown stays at 00:00 and `RUNNING`, Primary does nothing while Running, and Next does nothing. These are placeholders until tickets 06, 03 and 04, which will need to replace the tests that pin them.

All checks passed on the device: the layout and margins, the readability of the digits at desk distance, the contrast, and the countdown with no visible flicker. `DISPLAY_CONTRAST` stays at 64.
