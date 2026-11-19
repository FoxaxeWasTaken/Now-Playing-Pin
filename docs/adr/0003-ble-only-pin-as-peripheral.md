# ADR-0003: BLE only, pin as GATT peripheral

- Status: Accepted
- Date: 2026-11-19 (recorded; the decision is older)
- Related: [protocol.md](../protocol.md), [power.md](../power.md),
  [spike 0003](../spikes/0003-ble-bundle-transfer.md),
  [spike 0004](../spikes/0004-connection-interval-android.md)

## Context

The pin has to last 2 to 3 months on about 250 mAh, an average of roughly 90 to 140 µA
(estimate, not measured). It only needs about 4 kB per track change, and the phone is always
nearby.

## Decision

The only link is Bluetooth Low Energy. The pin is the GATT peripheral, the phone is the
central. The pin has no Wi-Fi and no Bluetooth Classic.

## Alternatives

- Wi-Fi on the pin, so that it fetches covers itself. Rejected to protect the power budget
  and because the phone already has the data.
- Bluetooth Classic. Rejected for the same power reason.

No measurement backs these rejections yet. They follow from the power target.

## Consequences

- Everything that needs a network happens on the phone.
- The app must stay alive in the background, which brings the foreground service, the
  companion device association and the battery optimisation exemption.
- A transfer is bounded by the MTU and the connection interval. How the connection is set up
  around a transfer is not decided yet.
- The connection parameters outside transfers are a power lever, and what Android grants is
  not known yet.
- A radio link to a screen worn in public has to be protected
  ([ADR-0012](0012-ble-security.md)).
