# 06: Overtime

**What to build:** When a Running Interval reaches its planned duration it does not end. It continues into Overtime, and its time keeps accruing to the Interval until the user presses Next. Overtime is shown as `+mm:ss` counting up with an `OVER` tag, and the progress bar is full. A countdown never carries a sign. This applies to Focus and Break alike, and Overtime can be paused and resumed like any other Running Interval. No alert styling yet (ticket 07). Spec: `.scratch/mvp/spec.md` (stories 7, 17–24).

**Blocked by:** 04, 05

**Status:** ready-for-agent

- [ ] Snapshot exposes whether the Interval is in Overtime
- [ ] Exactly at the planned duration the Interval enters Overtime; time shown switches from remaining to Overtime accrued
- [ ] Overtime accrues across pauses; Primary pauses and resumes during Overtime
- [ ] Overtime markers (`+`, `OVER`, full bar) remain while Paused in Overtime
- [ ] Next during Overtime (Running or Paused) → Ready, next Interval type
- [ ] Tested for both Focus and Break, including the exact boundary
- [ ] Rendered on the device as `+mm:ss` with `OVER`; verified by hand (shorten durations temporarily if useful, but commit the real constants)
