# ADR-0010: V1 scope

- Status: Accepted
- Date: 2026-11-19
- Related: [roadmap.md](../roadmap.md)

## Context

A review of the project on 2026-11-19 found no written definition of V1. Features described
as "quality of life" were already present in the protocol, the tests and the button map. For
one person working in spare time, the main risk is months of work before anything real runs.

## Decision

V1 is the smallest pin worth wearing:

- the cover view (cover, title, artist), following track changes;
- the share view with a QR code, toggled with KEY1;
- sleep and wake with the buttons;
- a measured battery life;
- the printed case.

Left out of V1 and listed under "Later" in the roadmap:

- the Spotify Code (fetch, protocol characteristic, view);
- media control from the pin (play/pause on KEY2, next on KEY3);
- every setting: rotation, inversion, discreet mode, custom sleep image, choice of apps;
- a preview in the app that reproduces every view of the pin;
- history, and NFC (V2).

The conditions that say V1 is done are in [roadmap.md](../roadmap.md).

## Alternatives

- Keeping the full feature list in V1. Rejected: it delays the first working pin and adds
  protocol messages that cannot be tested before the hardware exists.

## Consequences

- The V1 protocol has fewer messages. It carries a version number so that the later features
  can be added.
- The app needs no network permission and no settings screen in V1.
- KEY2 and KEY3 have no function in V1 other than the pairing gestures.
- The sleep image is a bitmap built into the firmware.
- Which apps are shown on the pin has to be a fixed rule in V1, since there is no setting
  ([ADR-0015](0015-only-chosen-apps-are-shown.md)).
