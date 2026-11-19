# ADR-0014: Arduino with the Seeed nRF52 core

- Status: Accepted
- Date: 2026-11-19 (recorded; the decision is older)
- Related: [firmware.md](../firmware.md), [hardware.md](../hardware.md),
  [ADR-0004](0004-nrf52840-en05-board.md), [ADR-0008](0008-portable-renderer-and-simulator.md)

## Context

The board is a Seeed product. Seeed documents it with Arduino: the "Seeed nRF52 Boards"
core, the Bluefruit BLE library that comes with it, and an Arduino display library.

## Decision

The firmware is an Arduino sketch built with the Seeed nRF52 core, using Bluefruit for BLE.

## Alternatives

- Zephyr or the Nordic SDK. Not evaluated. They give finer control over power and Bluetooth
  security, at the price of a much larger setup and no vendor example for this board.

## Consequences

- The vendor's examples and pin definitions can be used as they are.
- What Bluefruit allows for connection parameters, security and private addresses limits the
  design. These limits are not known yet.
- Because the rendering and protocol code is portable C++ with no Arduino dependency, moving
  to another framework later would only replace the thin hardware layer.
