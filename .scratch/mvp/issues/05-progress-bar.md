# 05: Progress bar

**What to build:** A progress bar along the bottom of the screen showing elapsed against planned time for the current Interval, so progress can be judged at a glance without reading digits. Spec: `.scratch/mvp/spec.md` (story 37).

**Blocked by:** 02

**Status:** ready-for-agent

- [ ] Snapshot exposes progress as elapsed against planned, capped at full
- [ ] Progress is empty in Ready and grows while Running
- [ ] Progress is unchanged while Paused (once ticket 03 is in)
- [ ] Rendered as a bar along the bottom of the screen, below the large digits, without crowding them
- [ ] Works for both Focus and Break (once ticket 04 is in)
- [ ] Verified by hand on the device
