# 10: Guard against an accidental Next while Running

**What to decide:** Today Next ends the current Interval from Ready, Running and Paused (spec stories 8–14, button table). The two buttons sit side by side, so it's easy to press Next when you meant Primary. A stray Next while Running or Paused throws away the Interval's elapsed time with no undo. Only the Ready case is safe, because pressing Next again toggles back (story 12). Decide how Next should behave while an Interval is in progress, then update the spec and the affected tickets before building it. Spec: `.scratch/mvp/spec.md` (stories 9, 13, 14, 19; button table; Out of Scope → "Button layout experiments").

**Blocked by:** 04

**Status:** needs-triage

## Why now

Ticket 04 implements Next as the spec describes. It was confirmed on the device, and this ticket doesn't change it. Ticket 06 (Overtime) will build in "Next while Running in Overtime ends the Interval" (story 19), and ticket 07 builds alert behaviour on top of that. Decide this before 06 starts, so its tests don't have to be rewritten.

## Options

### A. Ignore Next while Running, except in Overtime

- Running Focus or Break, before the planned duration: Next does nothing.
- To abandon an Interval early: Primary (pause), then Next.
- Running in Overtime: Next still ends the Interval, as story 19 says, so ending Overtime stays one press while the alert is on screen.
- Paused and Ready are unchanged.
- Spec impact: story 9 ("Next … never changes meaning") is dropped. Stories 13 and 14 still hold, but from Running they take an extra Primary press.

### B. Ignore Next while Running, including Overtime

- Next never ends a Running Interval, whether before or in Overtime. The clock has to be stopped first, and only then can Next move to the next Interval type.
- Ending Overtime becomes Primary (pause, which drops alert styling per story 28), then Next.
- Next behaves the same in every Running state, so the rule is simpler to state than A.
- Spec impact: as A, plus story 19 ("Overtime to end when I press Next") changes to need a pause first.
- **Open question:** what does Next do while Running?
  - B1: nothing at all.
  - B2: it stops the clock (→ Paused), the same as Primary.

  B2 means a stray Next costs nothing, because Primary resumes. Two deliberate Next presses then still end a Running Interval, which keeps stories 13 and 14 intact. B1 makes Next do nothing visible, which may feel like a dead button.
- **Open question:** after the clock is stopped, which press moves on? Next is the natural choice. Primary already means "resume" while Paused (story 4), so using Primary to advance would change the meaning of Primary too.

### Other ideas to consider

- **Hold-to-confirm:** Next acts only after being held for about a second while Running or Paused. A tap from Ready still toggles, so story 12 is unchanged. This is already listed under Out of Scope → "Button layout experiments", and it needs hold detection in the hardware layer.
- **Press twice to confirm, with a prompt:** the first Next while Running or Paused shows a prompt in the header (e.g. `NEXT?`) for about 3 seconds. A second Next within that window confirms. Primary, or waiting, cancels. The prompt is display-only, which fits story 32. The timer logic would need a new state or flag for the prompt, which could be tested like the flash window.
- **Undo:** Next acts at once, but a Next or Primary press within a few seconds restores the ended Interval with its elapsed time. This conflicts with the Next-from-Ready toggle (story 12), so the two would need reconciling.
- **Guard Paused as well as Running:** options A and B only protect a Running Interval, so a stray Next while Paused still ends the Interval. The prompt and hold-to-confirm options can cover both states.
- **Physical layout:** make the buttons different sizes or colours, space them apart, recess Next, or move it to the side of the case. This needs no logic change, but it needs the enclosure, which isn't designed yet.

## Interactions to check whichever option is chosen

- The Overtime limit ends an Interval "exactly as if Next were pressed". It isn't a button press, so the guard must not apply to it.
- A press while the display is blanked only wakes it (stories 39–43, ticket 08). A guarded Next while blanked should still just wake the display.
- Pausing drops alert styling (story 28), and resuming Overtime shows steady-inverted (story 29). Option B and the prompt route the end of Overtime through those paths, so check that the screen still feels calm.
- The Next-from-Ready toggle (story 12) should stay a single tap.

## Before building

- [ ] Choose an option, using a few real Focus/Break cycles on the device as evidence of how often accidental presses happen
- [ ] Update the spec: stories 9, 13, 14 and 19 as needed, the button table, and the glossary entry for Next if its meaning changes
- [ ] Update tickets 06 and 07 to match the chosen option. 06 is already blocked by this ticket; unblock it early only if the choice leaves Overtime unchanged
- [ ] Rewrite this ticket as a build ticket with acceptance criteria, then move it to `ready-for-agent`

## Comments
