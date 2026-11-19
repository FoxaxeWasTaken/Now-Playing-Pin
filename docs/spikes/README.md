# Spikes

A spike is a short, time-boxed experiment that answers one question, so that a decision can
be made on evidence. The process is defined in
[ADR-0001](../adr/0001-record-decisions-and-spikes.md).

## Index

| Spike | Question | Needs the pin | Status |
| --- | --- | --- | --- |
| [0001](0001-spotify-code-endpoint.md) | Does the Spotify Code endpoint work, and can the bars be extracted? | No | Planned |
| [0002](0002-metadata-event-timing.md) | In what order and how fast do the metadata events arrive? | No | Planned |
| [0003](0003-ble-bundle-transfer.md) | How does a bundle transfer behave from Android? | No (second phone) | Planned |
| [0004](0004-connection-interval-android.md) | Which connection parameters can the pin get from an Android phone? | Partly | Planned |
| [0005](0005-qr-size-and-scannability.md) | Is the planned QR code readable? | Partly | Planned |
| [0006](0006-app-preview-renderer.md) | How does the app preview stay identical to the pin? | No | Planned |
| [0007](0007-cad-tool.md) | OpenSCAD or CadQuery? | No | Planned |
| [0008](0008-board-idle-current.md) | How much does the board draw at rest? | Yes | Planned |
| [0009](0009-display-driver.md) | Does the display library drive this panel on the EN05? | Yes | Planned |
| [0010](0010-partial-refresh-ghosting.md) | Is partial refresh good enough on dithered covers? | Yes | Planned |
| [0011](0011-legibility-at-scale.md) | At what distance are the title and artist readable? | No | Planned |
| [0012](0012-pairing-and-reconnection.md) | Does passkey pairing hold, and does the phone still reconnect by itself? | Partly | Planned |

## Rules

- One question per spike.
- Status is one of: Planned, Running, Done, Abandoned.
- The time boxes are suggestions. When the time is up, the result is written with what is
  known, even if the answer is "not conclusive".
- The **Result** section is only filled from an experiment that was actually run, with the
  date and the conditions. It stays "Not run" until then.
- Code written for a spike is throwaway unless it is brought up to the project's rules.
- A spike that is done names the document it changed, and the ADR it led to if there is one.
- Something learned outside the experiment goes under "Notes" or "Known before starting",
  never under "Result".

## Template

```markdown
# Spike NNNN: Title

- Status: Planned
- Needs the pin: yes / no
- Time box:
- Feeds: ADR or document

## Question

## Why it matters

## Method

## Done when

## Result

Not run.

## Outcome
```
