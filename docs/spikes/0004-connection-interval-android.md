# Spike 0004: Connection parameters with an Android central

- Status: Planned
- Needs the pin: an nRF52840 board is enough for the parameters; the real board is needed
  for the currents
- Time box: one day (suggested)
- Feeds: [power.md](../power.md), [protocol.md](../protocol.md),
  [ADR-0003](../adr/0003-ble-only-pin-as-peripheral.md)

## Question

Which connection interval and slave latency can the pin actually obtain from an Android
phone, and what do they cost in current and in response time?

## Why it matters

The connection is one of the main items of the power budget, and the two options in
[protocol.md](../protocol.md) (a long interval, or a shorter one with slave latency) cannot
be chosen on paper. Android decides the parameters as the central.

## Known before starting

To confirm, not to rely on:

- Android's `LOW_POWER` preset is reported as an interval of 100 to 125 ms with a latency of
  2, and `HIGH` as 11.25 to 15 ms.
- Android is reported to accept a parameter request from the peripheral when it is within
  the limits of the Bluetooth specification. Phone makers may differ.
- A priority request from the phone replaces what the pin obtained.

## Method

1. With a minimal firmware, connect and log the parameters granted by default.
2. From the pin, request a long interval (option A), then a 100 ms interval with slave
   latency (option B), and log what Android answers each time.
3. From the phone, request each priority and log what happens to the pin's parameters, and
   how long the change takes to apply.
4. For each setting obtained, measure the average current while connected and idle.
5. For each setting, measure how long a bundle transfer takes, and how long the pin takes to
   reconnect after going out of range.
6. Repeat on a second Android phone if one is available.

## Done when

- A table gives, per setting: parameters granted, idle current, transfer time, reconnection
  time.
- One option is chosen, or the battery target is revised.

## Result

Not run.

## Outcome

To be written. Expected: the connection parameters fixed in protocol.md, rows of the
measurement table in power.md, and a revision of the battery estimate.
