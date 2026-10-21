# 07: Overtime alert

**What to build:** A display-only alert when an Interval enters Overtime. The screen flashes, alternating inverted and normal, for about 10 seconds, then stays steadily inverted while Running in Overtime. Pausing drops all alert styling and ends the flash for good; resuming in Overtime shows the steady inverted screen, not a restarted flash. Next ends any alert. Spec: `.scratch/mvp/spec.md` (stories 25–32).

**Blocked by:** 06

**Status:** ready-for-agent

- [ ] Snapshot exposes alert styling: none, flash-inverted, flash-normal, or steady-inverted
- [ ] Entering Overtime starts the flash; the on/off phase is derived from timestamps
- [ ] After the ~10 s flash window, styling is steady-inverted while Running in Overtime
- [ ] Paused (anywhere) has no alert styling
- [ ] Pausing mid-flash then resuming gives steady-inverted
- [ ] Next during any alert → Ready next type with no alert styling
- [ ] Flash window length is a named constant
- [ ] Hardware layer inverts the display according to the styling; verified by hand that the flash is noticeable but not a strobe
