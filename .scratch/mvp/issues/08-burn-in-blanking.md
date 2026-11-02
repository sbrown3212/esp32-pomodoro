# 08: Burn-in blanking

**What to build:** To protect the OLED, the display blanks after 5 minutes continuously in Ready or Paused, including Paused in Overtime. It never blanks while Running, including Overtime (the Overtime limit from ticket 06 bounds how long that can last: a forgotten Interval ends after 60 minutes of Overtime, becomes Ready, and then blanks under this rule). Any button press restarts the 5-minute countdown. The first press of either button while blanked only wakes the display and is otherwise ignored, so waking can't accidentally start, pause or skip anything. Spec: `.scratch/mvp/spec.md` (stories 39–43).

**Blocked by:** 04, 06

**Status:** ready-for-agent

- [ ] Snapshot exposes whether the display is blanked
- [ ] Blanks after 5 minutes continuously in Ready, and in Paused (including Paused in Overtime)
- [ ] Never blanks while Running, including Overtime
- [ ] An Interval ended by the Overtime limit becomes Ready and then blanks 5 minutes later
- [ ] Any press restarts the 5-minute countdown
- [ ] Primary or Next while blanked only wakes the display: run state, Interval type and time are unchanged
- [ ] Blanking delay is a named constant
- [ ] Hardware layer blanks the display when the snapshot says so; verified by hand
