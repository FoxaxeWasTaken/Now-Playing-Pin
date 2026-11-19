# Power

Status: **every figure here is an estimate.** Nothing has been measured.

## Target

2 to 3 months on a 250 mAh cell. With about 80 % of the capacity usable, that is an average
current of roughly 90 to 140 µA.

## Desk estimate

Made during a documentation review on 2026-11-19, from typical figures and assumptions. It
shows which items matter. It is not a prediction.

| Item | Assumption | Average |
| --- | --- | --- |
| Board at rest | Processor idle with its clock and memory kept, flash asleep, regulator and charger | 5 to 15 µA |
| Connection, idle | One short radio event every 0.4 to 1 s | 5 to 15 µA |
| Tracks | About 100 a day, each one a transfer and a partial refresh, a full refresh every 10 | About 12 µA |
| Cell self-discharge and protection circuit | 3 to 4 % a month | 10 to 15 µA |
| **Total** | | **About 40 to 60 µA, 100 µA if things go badly** |

What it suggests:

- The target looks reachable with some margin.
- **The board's idle current is the largest unknown.** The regulator, the charger and the
  battery measurement circuit of the EN05 are not documented. If the board draws 100 µA at
  rest, no firmware choice makes up for it.
  [Spike 0008](spikes/0008-board-idle-current.md) measures it first.
- The connection interval matters, but perhaps less than first assumed: even at a 100 ms
  interval without slave latency, the estimate for the connection stays near 40 to 50 µA.
  [Spike 0004](spikes/0004-connection-interval-android.md) measures it.
- A full refresh on every track instead of a partial one would add about 15 µA. It stays
  affordable if partial refresh gives a poor image.

## Levers

- **Board at rest.** Flash in deep power-down, display supply cut, nothing left powered
  between events. Known trap: the XIAO's QSPI flash draws about ten times more when it is
  not put to sleep.
- **Connection parameters.** Two options, described in [protocol.md](protocol.md).
- **Refresh count.** Refreshes happen on events only ([firmware.md](firmware.md)). The
  delays before an icon is drawn keep a burst of events from costing a refresh each.
- **Fewer transfers.** The app does not resend a bundle the pin already holds, and does not
  send one when nothing visible changed.
- **Disconnected state.** Slow advertising, 1 to 2 s.
- **Sleep state.** System OFF, wake on any button. Compared with staying idle, the saving
  on the processor is small; the real saving is that advertising stops.

## Battery monitoring

- The battery is measured once per track and published through the Battery Service.
- An icon appears on the pin below 20 %.

## Measurement plan

To run as soon as the hardware arrives, with a µA-capable multimeter or a Nordic PPK2:

| Measurement | Result |
| --- | --- |
| Board alone at rest, radio off | not measured |
| System OFF | not measured |
| Advertising, disconnected | not measured |
| Connected, idle, for each connection setting tried | not measured |
| One track transfer plus partial refresh | not measured |
| One full refresh | not measured |
| Charge current | not measured |
| Average over a real listening day | not measured |

## Rule

Anything that increases the pin's consumption is flagged and written down here before it is
added.
