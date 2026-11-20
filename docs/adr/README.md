# Architecture decision records

One file per decision that shapes the project. The process is defined in
[ADR-0001](0001-record-decisions-and-spikes.md).

## Index

| ADR | Title | Status |
| --- | --- | --- |
| [0001](0001-record-decisions-and-spikes.md) | Record decisions as ADRs and open questions as spikes | Accepted |
| [0002](0002-android-only.md) | Android only | Accepted |
| [0003](0003-ble-only-pin-as-peripheral.md) | BLE only, pin as GATT peripheral | Accepted |
| [0004](0004-nrf52840-en05-board.md) | nRF52840 board (EN05) rather than ESP32-S3 (EE05) | Accepted |
| [0005](0005-phone-renders-pin-displays.md) | The phone renders, the pin displays | Accepted |
| [0006](0006-one-bundle-per-track-refresh-on-events.md) | One bundle per track, refresh on events only | Accepted |
| [0007](0007-qr-share-view-spotify-code-later.md) | QR code as the share view, Spotify Code later | Accepted |
| [0008](0008-portable-renderer-and-simulator.md) | Portable renderer and PC simulator | Accepted |
| [0009](0009-parametric-enclosure-as-code.md) | Parametric enclosure as code | Accepted |
| [0010](0010-v1-scope.md) | V1 scope | Accepted |
| [0011](0011-manual-install.md) | The app is installed by hand | Accepted |
| [0012](0012-ble-security.md) | BLE security | Proposed |
| [0013](0013-enclosure-envelope.md) | A thicker case with the battery behind the board | Accepted |
| [0014](0014-arduino-seeed-nrf52-core.md) | Arduino with the Seeed nRF52 core | Accepted |
| [0015](0015-only-chosen-apps-are-shown.md) | Only chosen apps are shown | Accepted |
| [0016](0016-openscad-for-the-enclosure.md) | OpenSCAD for the enclosure | Accepted |

## Rules

- Files are named `NNNN-short-title.md`, numbered in order.
- Status is one of: Proposed, Accepted, Superseded by ADR-NNNN.
- An accepted ADR is not rewritten. To change a decision, write a new ADR and mark the old
  one as superseded.
- An ADR states what is known. A reason that rests on an estimate or on an unverified value
  says so.
- An ADR is for a choice that is costly to reverse. A smaller choice is recorded in the pull
  request that makes it.

## Template

```markdown
# ADR-NNNN: Title

- Status: Proposed
- Date: YYYY-MM-DD
- Related: ADRs, spikes, documents

## Context

What forces the decision. Facts, constraints, what is not known.

## Decision

What is decided, in one or two sentences.

## Alternatives

What else was considered and why it was not chosen.

## Consequences

What becomes easier, what becomes harder, what has to be watched.
```
