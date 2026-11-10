# 05: Progress bar

**What to build:** A progress bar along the bottom of the screen showing elapsed against planned time for the current Interval, so progress can be judged at a glance without reading digits. Spec: `.scratch/mvp/spec.md` (story 37).

**Blocked by:** 02

**Status:** needs-device-check

- [x] Snapshot exposes progress as elapsed against planned, capped at full
- [x] Progress is empty in Ready and grows while Running
- [x] Progress is unchanged while Paused (once ticket 03 is in)
- [x] Rendered as a bar along the bottom of the screen, below the large digits, without crowding them
- [x] Works for both Focus and Break (once ticket 04 is in)
- [ ] Verified by hand on the device

## Comments

The snapshot gives progress as `progressThousandths`, from 0 (empty) to 1000 (full). It is worked out from the shown whole seconds rather than the exact elapsed time, so it changes only when the shown second does, and the screen still redraws at most once a second. Capping at full comes from the countdown stopping at 00:00.

The hardware layer draws a 124 × 7 px outline on the 2 px bottom margin, at y 55–61. The fill sits inside it with a 1 px gap, up to 120 × 3 px. An empty bar is just the outline. A 2 px fill looked thin on the device, so the bar grew to 7 px. To keep space below the time, the time baseline moved up from 48 to 47. That leaves an even 8 px gap between the header and the digits (rows 11–18), and between the digits and the bar (rows 47–54).

Please check on the device:
- **Ready:** the bar is an empty outline and doesn't crowd the digits.
- **Running:** it fills from the left as a Focus runs, holds still while Paused, and empties again after Next.
- **Break:** it fills visibly faster, by about one pixel every 2.5 seconds.
