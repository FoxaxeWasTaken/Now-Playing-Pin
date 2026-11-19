# Spike 0012: Pairing and reconnection with security on

- Status: Planned
- Needs the pin: an nRF52840 board is enough; the passkey screen needs the panel
- Time box: one day (suggested)
- Feeds: [ADR-0012](../adr/0012-ble-security.md), [protocol.md](../protocol.md),
  [android-app.md](../android-app.md)

## Question

Can the firmware enforce passkey pairing at security level 4 and refuse everything else,
and does an Android phone still reconnect by itself to a pin that advertises almost nothing?

## Why it matters

ADR-0012 rests on what a review read in the library's source, not on anything tried. Two
things could break it: the stack not behaving as read, and the security measures (private
address, advertising without a name) preventing Android's companion presence from finding
the pin again.

## Method

1. Set the custom service and the battery level to level 4. Pair with the passkey and check
   that reads and writes work. Check that the passkey appears on the e-ink screen soon enough
   for the phone's pairing dialog.
2. From another device, pair without a passkey: check that reads, writes and subscriptions
   are refused and that the unwanted bond is deleted, while the real phone's bond is intact
   and the phone still reconnects.
3. Try to pair outside the pairing window: check that no new bond is kept, and again that
   the real phone's bond is intact.
4. Check that the list of services holds no update service, and that the device name cannot
   be written.
5. Once bonded, advertise without name or service UUID. Check that the phone reconnects by
   itself after a short loss of link, after the pin goes out of range for a long time, after
   the phone restarts and after the app is killed. Note which mechanism brought it back: the
   pending connection request, or the presence observation.
6. Turn on a resolvable private address and repeat step 5.
7. Erase the bond on the pin, then pair the same phone again. Note what has to be done on
   the phone first.
8. Wake the pin from System OFF with each button and check that it starts the firmware and
   not an update mode of the bootloader.
9. Measure what it costs when a stranger connects and is dropped after 5 s. Try the two
   refinements of ADR-0012: encryption requested by the pin at once, and an accept list
   based on the phone's identity key.

## Done when

- Each point of ADR-0012 has a yes or a no on the hardware.
- The advertising content and the address type are chosen.

## Result

Not run.

## Outcome

To be written. Expected: ADR-0012 accepted as it is, or replaced by a new ADR; the security
section of protocol.md and the connection lifecycle of android-app.md updated.
