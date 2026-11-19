# ADR-0011: The app is installed by hand

- Status: Accepted
- Date: 2026-11-19
- Related: [android-app.md](../android-app.md), [ADR-0002](0002-android-only.md)

## Context

The app is used by one person with one pin. How it is distributed changes what it may do:
a store has policies on notification access, battery optimisation exemption and foreground
services, and Android applies restrictions to apps installed outside a store.

## Decision

The app is built from this repository and installed by hand. It is not published on a store.

## Alternatives

- Publishing on Google Play. Not needed for one user, and it adds policy constraints on the
  exact permissions this app relies on.

## Consequences

- No store review, no store policy to satisfy.
- Android treats notification access as a restricted setting for an app installed this way:
  the user allows restricted settings for the app once, by hand **[to verify on the phone]**.
- Anyone else who wants the app builds it themselves.
- If the app is ever published, this decision and the list of permissions are reviewed.
