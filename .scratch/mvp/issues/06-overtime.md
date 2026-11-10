# 06: Overtime

**What to build:** When a Running Interval reaches its planned duration it does not end. It continues into Overtime, and its time keeps accruing to the Interval until the user presses Next, or until Overtime reaches the 60-minute Overtime limit, at which point the Interval ends exactly as if Next were pressed. Overtime is shown as `+mm:ss` counting up with an `OVER` tag, and the progress bar is full. A countdown never carries a sign. This applies to Focus and Break alike, and Overtime can be paused and resumed like any other Running Interval. No alert styling yet (ticket 07). Spec: `.scratch/mvp/spec.md` (stories 7, 17–24).

**Blocked by:** 04, 05, 09, 10

**Status:** ready-for-agent

- [ ] Snapshot exposes whether the Interval is in Overtime
- [ ] Exactly at the planned duration the Interval enters Overtime; time shown switches from remaining to Overtime accrued
- [ ] Overtime accrues across pauses; Primary pauses and resumes during Overtime
- [ ] Overtime markers (`+`, `OVER`, full bar) remain while Paused in Overtime
- [ ] Next during Overtime (Running or Paused) → Ready, next Interval type
- [ ] At exactly 60 minutes of accrued Overtime the Interval ends → Ready, next Interval type; paused time does not count toward the limit; works for Focus and Break
- [ ] The Overtime limit is a named constant
- [ ] Tested for both Focus and Break, including the exact boundary
- [ ] Rendered on the device as `+mm:ss` with `OVER`; verified by hand (shorten durations temporarily if useful, but commit the real constants)

## Comments

Ticket 02 added `test_countdown_stops_at_zero_at_and_after_the_planned_duration`, which pins the countdown stopping at 00:00. Overtime replaces that behaviour, so rewrite the test for entering Overtime rather than keeping it passing. Also, `draw()` centres the time on the width of a fixed `00:00`, so `+mm:ss` will need its own fixed centring width.

Ticket 05 works out progress as `(planned seconds − shown seconds) / planned seconds`, and it is full only because the countdown stops at 00:00. Once the shown time becomes Overtime accrued, that sum gives wrong values. Make progress full in Overtime explicitly (story 22). `test_progress_is_full_at_and_after_the_planned_duration` will need its "after" case to stay at full.
