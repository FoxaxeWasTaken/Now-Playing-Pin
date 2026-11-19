# Spike 0008: Idle current of the board

- Status: Planned
- Needs the pin: yes (the board and a battery; the panel is not required)
- Time box: half a day (suggested)
- Feeds: [power.md](../power.md), [ADR-0004](../adr/0004-nrf52840-en05-board.md)

## Question

How much current does the complete EN05 board draw from the battery when the firmware does
nothing, with the radio off?

## Why it matters

The desk estimate in [power.md](../power.md) assumes 5 to 15 µA at rest. The regulator, the
charger and the battery measurement circuit of the board are not documented. If the board
draws much more, the battery target cannot be met by any firmware choice, and it is better
to know before writing the BLE code.

## Method

1. Power the board from the battery connector through the measuring instrument.
2. Flash a minimal firmware: no radio, flash put in deep power-down, display supply off,
   processor waiting for an event.
3. Measure the current. Then measure again with each item left on in turn: flash awake,
   display supply on, battery measurement enabled.
4. Measure System OFF.
5. Measure with the power switch off.

## Done when

A table gives the current for each case, with the instrument and the firmware used.

## Result

Not run.

## Outcome

To be written. Expected: the first rows of the measurement table in power.md, a revised
battery estimate, and a new ADR if the board cannot meet the target.
