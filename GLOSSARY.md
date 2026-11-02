# Pomodoro Timer

A desktop device that paces work using the Pomodoro technique: alternating timed periods of focused work and rest.

## Language

**Pomodoro**:
The technique, or this product. Never a unit of time.
_Avoid_: using "pomodoro" for a single work period

**Interval**:
Any single timed period on the device: either a Focus or a Break.
_Avoid_: Session, period, phase

**Focus**:
An Interval of concentrated work.
_Avoid_: Pomodoro, work session, sprint

**Break**:
An Interval of rest that follows a Focus.
_Avoid_: Rest, pause

## Run states

**Ready**:
An Interval is loaded at its full duration and waiting to be started.
_Avoid_: Idle, stopped

**Running**:
The current Interval's time is accruing.

**Paused**:
The current Interval is halted partway through, keeping its elapsed time.
_Avoid_: Stopped

**Overtime**:
The part of an Interval after its planned duration has elapsed. The Interval is still in progress, Running or Paused, and its time keeps accruing until the user ends it or it reaches the Overtime limit.
_Avoid_: Expired, finished, done

**Overtime limit**:
The most Overtime an Interval can accrue before it ends automatically, as if Next were pressed.
_Avoid_: Cap, timeout

## Actions

**Next**:
End the current Interval and make the next Interval type Ready.
_Avoid_: Skip, end, reset
