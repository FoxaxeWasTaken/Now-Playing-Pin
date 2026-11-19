# ADR-0002: Android only

- Status: Accepted
- Date: 2026-11-19 (recorded; the decision is older)
- Related: [android-app.md](../android-app.md)

## Context

The companion app has to know what any music app is playing, hold a BLE link in the
background for days, and control playback. The design relies on Android mechanisms for each
of these:

- `NotificationListenerService` and `MediaSessionManager` to read the metadata and cover of
  any music app;
- the Spotify broadcast `com.spotify.music.metadatachanged` to get the track id;
- `CompanionDeviceManager` and a `connectedDevice` foreground service to keep the link alive.

The project is built by one person who uses an Android phone.

## Decision

The companion app targets Android only. No iOS version, and no abstraction layer kept "in
case".

## Alternatives

- iOS support. Out of scope. It was not evaluated, so nothing here says whether it would be
  feasible.

## Consequences

- The app is plain Kotlin and can use Android APIs directly.
- The pin is not usable with an iPhone.
- The BLE protocol itself contains nothing specific to Android.
