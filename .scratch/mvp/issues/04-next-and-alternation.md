# 04: Next and Interval alternation

**What to build:** Next ends the current Interval and makes the next Interval type Ready, from Ready, Running or Paused, and never starts the clock. Focus (25:00) and Break (5:00) alternate. Pressing Next repeatedly from Ready toggles Focus ↔ Break, so an accidental press is undone by pressing again. A full Focus → Break → Focus loop is possible on the device. Spec: `.scratch/mvp/spec.md` (stories 8–16, button table).

**Blocked by:** 03

**Status:** needs-device-check

- [x] Next from Ready, Running and Paused → Ready, next Interval type, at full planned duration
- [x] Next never starts the clock
- [x] Interval types alternate Focus → Break → Focus
- [x] Next twice from any Focus gives a Ready Focus at 25:00
- [x] Every cell of the spec's button table (excluding blanked and Overtime, covered later) is tested in both Focus and Break
- [x] Pause and resume behave identically in a Break
- [x] Next button is wired through the hardware layer; header shows `BREAK` during a Break
- [ ] Verified by hand: Focus → Next → Break → Next → Focus on the device

## Comments

Ticket 03 banks a paused Interval's elapsed time in the timer logic, and starting from Ready only works because that is zero at power-on. Next must clear it, or a fresh Ready Interval will start with the old Interval's elapsed time (story 15).

Next toggles the Interval type, returns to Ready and clears the banked elapsed time, so a Ready Interval is always at its full planned duration. The planned duration now follows the Interval type. The hardware layer already called `nextPressed` and drew `BREAK` for a Break, so `src/main.cpp` is unchanged. The new behaviour comes from the snapshot now carrying the real Interval type. Tests cover all six button-table cells, outside blanking and Overtime, for both Focus and Break. They also cover the Next toggle from Ready, Next twice from Running and from Paused, and a full Focus → Break → Focus loop. The firmware builds, but it has not run on a board.

Please check on the device: from a Ready Focus, press Next to see `BREAK` `READY` 05:00, then press Next again to see `FOCUS` `READY` 25:00. Start a Focus, press Next while it is Running and again while it is Paused, and check that each press gives a Ready Interval at full time that doesn't count down. Then run Focus → Next → Break → Next → Focus.
