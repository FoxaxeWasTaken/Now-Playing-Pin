# ADR-0006: One bundle per track, refresh on events only

- Status: Accepted
- Date: 2026-11-19 (recorded; the decision is older)
- Related: [protocol.md](../protocol.md), [firmware.md](../firmware.md), [power.md](../power.md)

## Context

Battery life comes before features. The two expensive events on the pin are radio activity
and screen refreshes. An e-ink screen keeps its image without power, so the pin only needs
to act when something actually changes.

## Decision

- Everything is driven by events. Neither the app nor the pin polls.
- On each track change the phone sends one complete bundle: cover, text and share link.
- The pin keeps the bundle in RAM. Switching between the cover view and the share view is
  local and never contacts the phone.
- The screen is refreshed only when what it shows has to change: a new track (once, when the
  whole bundle has been received and checked), a change of state that adds or removes an
  icon, a button press that changes the view.
- No progress bar and no periodic refresh.

## Alternatives

- Fetching the share data only when the share view is opened. Rejected: it wakes the radio
  on a button press and makes the view depend on the phone being reachable.
- A progress bar or a clock. Rejected: it needs periodic refreshes.

## Consequences

- The pin never shows the position in the track.
- The share view still works when the phone is out of range, for the last track received.
- State icons (paused, disconnected) cost refreshes beyond one per track. They are drawn
  after a delay so that a burst of events costs one refresh.
- The bundle is in RAM, so it is lost in System OFF. After a wake, the pin keeps the sleep
  image until the phone sends the current track again, which the connection sequence does
  by itself.
