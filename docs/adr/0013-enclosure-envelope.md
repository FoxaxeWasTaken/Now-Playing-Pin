# ADR-0013: A thicker case with the battery behind the board

- Status: Accepted
- Date: 2026-11-19
- Related: [enclosure.md](../enclosure.md), [hardware.md](../hardware.md),
  [ADR-0009](0009-parametric-enclosure-as-code.md)

## Context

The first target for the case was about 31 × 61 mm and 10 to 12 mm thick. A stack-up made
during a review on 2026-11-19 gives about 10.4 mm before any battery is added, and a
footprint that leaves no room for the walls and the fold of the display cable. The heights
used are assumptions: nothing has been measured.

Two ways out: a thicker case with the battery stacked behind the board, or a longer case
with the battery beside the board.

## Decision

The case is thicker and keeps the footprint of the panel: about 33 × 64 × 14 mm, with a
thin cell (about 3 mm) stacked behind the board.

## Alternatives

- A longer case, about 33 × 85 × 11 mm, with the battery beside the board. Rejected: the pin
  would be noticeably longer than its screen.
- A smaller battery. Not chosen: it trades battery life, the first priority, for thickness.

## Consequences

- The battery has to be a thin cell, which narrows the choice.
- A cell as long as the board would lie over the XIAO module and its BLE antenna. A LiPo
  pouch is a sheet of metal and can detune an antenna placed against it. If the antenna has
  to stay clear, the cell is shorter and holds less, perhaps about 150 mAh. This is checked
  with the real board, by measuring the range with the cell in place.
- The numbers are estimates until the parts are measured. The parametric model absorbs the
  corrections.
- A pin about 14 mm thick stands out more from the fabric and puts more leverage on the
  magnet clip. Holding on thick fabric is checked before wearing it outdoors.
