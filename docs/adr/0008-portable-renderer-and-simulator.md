# ADR-0008: Portable renderer and PC simulator

- Status: Accepted
- Date: 2026-11-19 (recorded; the decision is older)
- Related: [firmware.md](../firmware.md), [testing.md](../testing.md)

## Context

The hardware is not available, and when it is there will be one unit. Flashing a board to
judge a layout is slow, and an e-ink panel cannot be captured in a test. Most of what the
firmware does is drawing: layout, QR code, badges.

## Decision

- The rendering code is C++ with no Arduino dependency. It draws into a 1-bit framebuffer.
  It lives in `firmware/render/`.
- `firmware/sim/` is a PC executable that reads test packets and writes the framebuffer as an
  image (PNG or PBM).
- The Arduino sketch only moves the framebuffer to the panel.

## Alternatives

- Drawing directly with the vendor's display library. Rejected: the code would only run
  on the board and could not be tested on a PC.

## Consequences

- The views can be designed and tested before the hardware arrives.
- Golden image tests run in CI.
- The renderer cannot use the display library's drawing or font functions.
- The framebuffer costs RAM on the pin: 250 × 122 pixels at 1 bit is about 4 kB.
