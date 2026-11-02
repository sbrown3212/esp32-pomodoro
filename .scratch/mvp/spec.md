# MVP: desktop Pomodoro timer

Status: ready-for-agent

Terms follow `GLOSSARY.md`. Toolchain rationale: `docs/adr/0001-platformio-arduino-framework.md`.

## Problem Statement

I want a dedicated, physical Pomodoro timer on my desk. Phone and computer timers pull me into the very device I'm trying not to get distracted by, and they make it easy to ignore or lose track of the end of a Focus. I want something I can start with one press, glance at to see where I am, and that tells me clearly when an Interval's planned time is up, without forcing me to stop if I'm in the middle of a thought.

## Solution

A small standalone device: an ESP32 DevKit V1 with a 128×64 OLED and two buttons, powered over USB, with no connectivity. It alternates 25-minute Focus and 5-minute Break Intervals. **Primary** always starts or stops the clock; **Next** always ends the current Interval and readies the next type. When an Interval reaches its planned duration it continues into Overtime, counting up visibly with an on-screen alert, until I press Next. If I've forgotten it and Overtime reaches 60 minutes (the Overtime limit), the Interval ends by itself and the next type becomes Ready. Intervals never start by themselves. The display blanks when nothing is happening, to protect the OLED from burn-in.

## User Stories

### Starting and the clock

1. As a user, I want the device to power on showing a Ready Focus at 25:00, so that I can start working with a single press.
2. As a user, I want Primary to start a Ready Interval, so that starting is one deliberate press.
3. As a user, I want Primary to pause a Running Interval, so that I can handle an interruption without losing my place.
4. As a user, I want Primary to resume a Paused Interval from exactly where it stopped, so that interruptions don't cost or gift me time.
5. As a user, I want Primary to mean "start or stop the clock" in every state, so that I never have to think about what it will do.
6. As a user, I want pausing and resuming to work identically in Focus and Break, so that the device behaves consistently.
7. As a user, I want to be able to pause an Interval that is in Overtime, so that interruptions during Overtime are handled like any other.

### Moving between Intervals

8. As a user, I want Next to end the current Interval and make the next Interval type Ready, so that I control when I move on.
9. As a user, I want Next to work the same from Ready, Running and Paused, so that it never changes meaning.
10. As a user, I want Next never to start the clock, so that I always choose when the next Interval begins.
11. As a user, I want Focus and Break to alternate, so that every Focus is followed by a Break and vice versa.
12. As a user, I want pressing Next from Ready to toggle between a Ready Focus and a Ready Break, so that an accidental press is undone by pressing again.
13. As a user, I want to be able to abandon a Focus early with Next and get a Ready Break, so that I can stop when a Focus isn't working.
14. As a user, I want to get back to a fresh Ready Focus from any Focus by pressing Next twice, so that I can restart a Focus without a separate reset button.
15. As a user, I want a Ready Interval to always be at its full planned duration, so that I know exactly what I'm starting.
16. As a user, I want the next Interval to wait in Ready rather than start automatically, so that my Breaks don't drift out of sync with what I'm actually doing.

### Overtime

17. As a user, I want a Running Interval that reaches its planned duration to continue into Overtime instead of ending, so that I can finish my thought without the device deciding for me.
18. As a user, I want Overtime time to keep accruing to the Interval, so that the Interval's true length is what I actually spent.
19. As a user, I want Overtime to end when I press Next, or automatically when it reaches the Overtime limit of 60 minutes (leaving the next Interval type Ready), so that I decide when I'm done, a forgotten timer doesn't run forever, and I'm nudged to take a break instead of sitting for hours.
20. As a user, I want Overtime shown as `+mm:ss` counting up with an `OVER` tag, so that I never mistake "two minutes over" for "two minutes left".
21. As a user, I want a countdown never to carry a sign, so that a `+` always and only means Overtime.
22. As a user, I want the progress bar to be full during Overtime, so that I have a second cue that the planned time is used up.
23. As a user, I want a paused Interval in Overtime to keep its `+`, `OVER` and full bar, so that I can tell I'm over even while paused.
24. As a user, I want Overtime to apply to Breaks as well as Focus, so that an overlong Break is just as visible.

### Alerts

25. As a user, I want the screen to flash, alternating inverted and normal, for about 10 seconds when an Interval enters Overtime, so that I notice the planned time is up.
26. As a user, I want the flash to settle into a steady inverted screen afterwards, so that the alert stays noticeable without being a strobe.
27. As a user, I want the steady inverted screen to remain while Overtime is Running, so that a glance always tells me I'm over.
28. As a user, I want pausing to drop all alert styling, so that a Paused screen looks the same in every situation.
29. As a user, I want resuming an Interval in Overtime to show the steady inverted screen rather than restarting the flash, so that resuming isn't jarring.
30. As a user, I want pausing during the 10-second flash to end the flash, so that the flash happens once per Interval.
31. As a user, I want Next to end any alert immediately, so that moving on gives me a calm screen.
32. As a user, I want alerts to be display-only, with no sound, so that the device doesn't disturb people around me.

### Screen

33. As a user, I want the Interval type (`FOCUS` / `BREAK`) shown in the header, so that I always know what I should be doing.
34. As a user, I want the run state (`READY` / `RUNNING` / `PAUSED`) shown in the header, so that I can tell at a glance whether the clock is going.
35. As a user, I want the time in large digits, so that I can read it from across my desk.
36. As a user, I want the time shown as `mm:ss`, so that I can see precise progress.
37. As a user, I want a progress bar of elapsed against planned time, so that I can judge how far through I am without reading digits.
38. As a user, I want the countdown to update every second without visible flicker, so that the display is calm to look at.

### Burn-in protection

39. As a user, I want the display to blank after 5 minutes continuously in Ready, so that the OLED doesn't burn in while I'm away.
40. As a user, I want the display to blank after 5 minutes continuously in Paused, including Paused in Overtime, so that a forgotten pause doesn't burn in the screen.
41. As a user, I want the display never to blank while Running, including Overtime, so that I never lose sight of an active Interval (the Overtime limit bounds how long that can last).
42. As a user, I want the first press of either button while blanked only to wake the display, so that waking it can't accidentally start, pause or skip anything.
43. As a user, I want the 5-minute blanking timer to restart whenever I press a button, so that the screen only blanks when I've genuinely left it alone.

### Hardware and development

44. As a user, I want the device to run from USB power alone, so that it just sits on my desk plugged in.
45. As a user, I want the device to need no Wi-Fi or Bluetooth, so that it works anywhere with no setup.
46. As a developer, I want all timer behaviour testable on my computer without the board, so that I and AI agents can iterate quickly and safely.
47. As a developer, I want pin assignments defined in one place, so that moving to another board (e.g. an ESP32-C3) later is a contained change.
48. As a developer, I want the durations defined as constants in one place, so that on-device settings can replace them later without restructuring.

## Implementation Decisions

### Hardware

- Board: ESP32 DevKit V1 (ESP32-WROOM-32), USB powered.
- Display: SSD1306 128×64 over I2C, SDA GPIO21, SCL GPIO22.
- Buttons: two momentary switches between GPIO and GND, using internal pull-ups. Primary on GPIO25, Next on GPIO26. Avoid strapping pins (0, 2, 12, 15) and input-only pins (34–39).
- All pin assignments live in a single place.

### Toolchain

- PlatformIO with the Arduino framework (ADR 0001), using the U8g2 display library.
- Two build environments: the ESP32 board, and native (host) for tests.

### Modules

**Timer logic** (the deep module; holds all behaviour). Pure C++, with no dependency on Arduino, the display, GPIO or a clock.

- Inputs: *Primary pressed*, *Next pressed* and *tick*. Each one carries the current monotonic time as a 64-bit millisecond timestamp, so the module never reads a clock itself and timestamps never roll over. Tick is called frequently so time-driven changes (entering Overtime, the flash ending, reaching the Overtime limit, blanking) happen with no press.
- Output: a snapshot describing what should be shown:
  - Interval type: Focus or Break
  - run state: Ready, Running or Paused
  - whether the Interval is in Overtime
  - the time to show, in whole seconds: remaining, or Overtime accrued
  - progress, elapsed against planned and capped at full
  - alert styling: none, flash-inverted, flash-normal or steady-inverted
  - whether the display is blanked
- It owns every rule:
  - button transitions
  - alternating types
  - entering Overtime at the planned duration
  - the ~10 s flash window and its on/off phase
  - pausing dropping alert styling, and the flash ending once interrupted
  - resuming in Overtime giving steady-inverted
  - Next ending alerts
  - 5-minute blanking in Ready or Paused, reset by any press
  - the Overtime limit: when accrued Overtime (excluding paused time) reaches 60 minutes, the Interval ends exactly as if Next were pressed, giving the next Interval type Ready with no alert styling; this applies to Focus and Break alike
  - a press while blanked only waking the display
- Durations (Focus 25 min, Break 5 min), the flash window (~10 s), the blanking delay (5 min) and the Overtime limit (60 min) are named constants.

**Hardware layer** (thin, with no decisions).

- It debounces both buttons with a library and turns presses into *Primary pressed* / *Next pressed* calls carrying the current time, taken from the ESP32's 64-bit clock (not the 32-bit `millis()`, which wraps after ~49.7 days of uptime).
- It calls *tick* every loop iteration.
- It renders the snapshot with U8g2:
  - header row with the Interval type on the left and the run state on the right
  - large time digits, or `+mm:ss` with an `OVER` tag in Overtime
  - progress bar along the bottom
  - inversion according to the alert styling
  - a blank screen when the snapshot says it's blanked
- It redraws only when the snapshot changes, to avoid flicker.

### Button behaviour

| State | Primary | Next |
|---|---|---|
| Ready | → Running | → Ready, next type |
| Running (incl. Overtime) | → Paused | → Ready, next type |
| Paused (incl. Overtime) | → Running | → Ready, next type |
| Display blanked | wake only | wake only |

- Short presses only; each acts on press after debounce. No long-press, double-press or chords.
- No Reset action. Next twice achieves it.

### Time

- Elapsed time is measured from monotonic 64-bit millisecond timestamps, accumulated across pauses. It is not counted in ticks, so missed or irregular ticks never cause drift.
- Displayed seconds round consistently, so the countdown shows 25:00 at start and Overtime begins exactly at the planned duration.

## Testing Decisions

- **One seam:** the timer logic's interface. Tests send *Primary pressed*, *Next pressed* and *tick* with chosen timestamps, then assert on the snapshot.
- **Good tests** assert only on externally visible behaviour, meaning snapshot fields after a sequence of inputs, never on internal fields or how state is stored. Each test reads as a scenario, e.g. "start a Focus, advance 25:01, the snapshot is Running, in Overtime, showing 1 second, alert flash".
- **Time is fully controlled by tests** through the timestamps passed in, so long durations (25 minutes, the 5-minute blanking) are tested instantly.
- **Coverage expectations:**
  - every cell of the button table, in both Focus and Break
  - type alternation and the Next-from-Ready toggle
  - pause and resume preserving elapsed time exactly
  - the boundary into Overtime, including exactly at the planned duration
  - Overtime accrual, including across a pause
  - flash phases during the window, then steady-inverted
  - pausing mid-flash, then resuming (steady-inverted)
  - Next during an alert
  - blanking after 5 minutes in Ready and in Paused, never while Running before Overtime
  - the Overtime limit: reaching exactly 60 minutes of Overtime gives the next type Ready, paused time doesn't count toward it, and it works for Focus and Break
  - large timestamps (as after weeks of uptime) still give correct times
  - the blanking timer restarting on a press
  - a wake press being swallowed
  - irregular tick spacing producing correct times
- **Tested module:** the timer logic only, run in the native environment with PlatformIO's test runner (Unity), developed test-first.
- **Not unit-tested:** the hardware layer (debouncing, drawing, pins). It is verified by hand on the device with a full Focus → Overtime → Next → Break → Focus run, checking layout, readability, alert appearance and the absence of flicker.
- **Prior art:** none. The repo is greenfield, so these tests set the pattern.

## Out of Scope

Listed roughly in priority order where known. Each becomes its own spec or ticket when picked up.

- **Long Break and Cycle**: a Long Break after every 4th Focus. Adds *Cycle* and *Long Break* to the glossary and renames Break to *Short Break*.
- **Focus count**: completed Focus Intervals since power-on, shown in the header. A Focus counts as completed once it reaches its planned duration, whatever is pressed afterwards.
- **Interval logging**: recording each Interval's actual length, including Overtime. Must decide how to record an Interval ended by the Overtime limit, whose true end is unknown.
- **Escalating alerts**: e.g. a further alert as the Overtime limit approaches or is reached, likely paired with the buzzer.
- **Flowtime-style mode**: Focus counts up from zero with no planned length, and the Break is sized from the Focus length.
- **On-device duration settings.**
- **Button layout experiments**: a Reset action, a third button, long-press / double-press / hold-to-repeat gestures (e.g. for settings menus), hold-to-confirm against accidental presses.
- **Buzzer alert.**
- **Battery power and deep sleep.**
- **ESP32-C3 port.**
- **Wi-Fi / Bluetooth connectivity.**

## Further Notes

- The ESP32-C3 can only wake from deep sleep on GPIO0–5. When battery support arrives, the wake button should sit on one of those pins so the C3 port stays easy.
- The Overtime limit exists because 60 minutes of Overtime almost always means the timer was forgotten, and because the device's purpose is to make me take breaks. It also bounds burn-in: the inverted Overtime screen lasts at most 60 minutes, after which the device is Ready and the normal 5-minute blanking applies. That's why there is no separate Overtime blanking rule. Because Overtime never exceeds 60:00, `+mm:ss` is always enough.
- An earlier revision blanked the display after 30 minutes of Overtime instead of limiting Overtime; it was replaced by the Overtime limit.
- The header has room on its right-hand side for the Focus count later without reworking the layout.
