# 04: Next and Interval alternation

**What to build:** Next ends the current Interval and makes the next Interval type Ready, from Ready, Running or Paused, and never starts the clock. Focus (25:00) and Break (5:00) alternate. Pressing Next repeatedly from Ready toggles Focus ↔ Break, so an accidental press is undone by pressing again. A full Focus → Break → Focus loop is possible on the device. Spec: `.scratch/mvp/spec.md` (stories 8–16, button table).

**Blocked by:** 03

**Status:** ready-for-agent

- [ ] Next from Ready, Running and Paused → Ready, next Interval type, at full planned duration
- [ ] Next never starts the clock
- [ ] Interval types alternate Focus → Break → Focus
- [ ] Next twice from any Focus gives a Ready Focus at 25:00
- [ ] Every cell of the spec's button table (excluding blanked and Overtime, covered later) is tested in both Focus and Break
- [ ] Pause and resume behave identically in a Break
- [ ] Next button is wired through the hardware layer; header shows `BREAK` during a Break
- [ ] Verified by hand: Focus → Next → Break → Next → Focus on the device

## Comments

Ticket 03 banks a paused Interval's elapsed time in the timer logic, and starting from Ready only works because that is zero at power-on. Next must clear it, or a fresh Ready Interval will start with the old Interval's elapsed time (story 15).
