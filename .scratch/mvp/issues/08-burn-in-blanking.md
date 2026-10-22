# 08: Burn-in blanking

**What to build:** To protect the OLED, the display blanks after 5 minutes continuously in Ready or Paused, including Paused in Overtime. It never blanks while Running before Overtime. If an Interval has been Running in Overtime for 30 continuous minutes (the user has probably walked away), the display also blanks, but the Interval keeps accruing. Overtime is never capped, so waking shows the true Overtime. Any button press restarts the 5-minute countdown. The first press of either button while blanked only wakes the display and is otherwise ignored, so waking can't accidentally start, pause or skip anything. Spec: `.scratch/mvp/spec.md` (stories 39–43).

**Blocked by:** 04, 06

**Status:** ready-for-agent

- [ ] Snapshot exposes whether the display is blanked
- [ ] Blanks after 5 minutes continuously in Ready, and in Paused (including Paused in Overtime)
- [ ] Never blanks while Running before Overtime
- [ ] Blanks after 30 minutes continuously Running in Overtime, counted from entering Overtime; pausing and resuming restarts that 30-minute count
- [ ] Overtime keeps accruing while blanked and is never capped; waking shows the true Overtime
- [ ] Any press restarts the 5-minute countdown
- [ ] Primary or Next while blanked only wakes the display: run state, Interval type and time are unchanged
- [ ] Idle blanking delay (5 min) and long-Overtime blanking delay (30 min) are named constants
- [ ] Hardware layer blanks the display when the snapshot says so; verified by hand
