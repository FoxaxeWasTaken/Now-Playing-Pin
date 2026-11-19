# ADR-0015: Only chosen apps are shown

- Status: Accepted
- Date: 2026-11-19
- Related: [android-app.md](../android-app.md), [ADR-0010](0010-v1-scope.md)

## Context

The pin is worn in public. The phone reports a media session for anything that plays sound
with one: music, but also a podcast, a video, sometimes a voice message. Whatever the app
sends to the pin is displayed on the wearer's chest.

Two rules were possible: show everything except a list of apps to ignore, or show only a
list of chosen apps.

## Decision

The app only shows what comes from a list of chosen apps. Anything else is ignored, as if
nothing was playing.

In V1 there is no settings screen, so the list is a constant in the code, set by the
maintainer. Choosing the apps from the app itself is planned for later.

## Alternatives

- Show everything except a list of ignored apps. Rejected: an app nobody thought of would be
  displayed in public by default.

## Consequences

- Nothing is displayed by surprise from an app that was not chosen.
- The rule is per app, not per kind of content: a podcast played in a chosen music app is
  shown like a track.
- A new music app shows nothing until it is added to the list. In V1 that means a new build
  of the app.
- The list is not written yet. It is filled when the app is scaffolded, with what
  [spike 0002](../spikes/0002-metadata-event-timing.md) shows about each app.
