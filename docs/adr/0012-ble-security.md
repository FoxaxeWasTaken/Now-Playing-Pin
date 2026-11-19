# ADR-0012: BLE security

- Status: Proposed
- Date: 2026-11-19
- Related: [protocol.md](../protocol.md), [firmware.md](../firmware.md),
  [android-app.md](../android-app.md),
  [spike 0012](../spikes/0012-pairing-and-reconnection.md)

## Context

The pin is a screen worn in public that displays what it receives over the air. Without
protection, anyone nearby could connect and write an image or a link to it: an offensive
picture, or a QR code that leads somewhere harmful.

A security review on 2026-11-19 read the source of the Bluefruit library (Seeed's fork,
main branch). Nothing has been tried on the hardware, and what the review found has to be
confirmed there **[to verify]**:

- Bluefruit accepts every pairing request and stores every bond. It has no way to refuse one.
- Its advertising filter cannot be restricted to one phone, and Android connects with a
  private address anyway.
- Its ready-made battery service is readable without encryption, and its device name is
  writable without encryption.
- Its DFU service, when started, has no protection, and the bootloader's over-the-air update
  is not signed by default.
- It has no function for a private advertising address. The underlying Nordic call has to be
  used directly.

So "pairing only during a window" and "only my phone" cannot rest on the library. They have
to rest on the security level that the Bluetooth stack itself enforces.

## Threats

Ordered by how likely they are to matter for a personal project:

1. Private content displayed by the wearer's own phone (a browser, a video, a voice
   message). Answered by [ADR-0015](0015-only-chosen-apps-are-shown.md).
2. An image or a link written by a stranger in radio range. Answered below.
3. A firmware update service exposed by mistake. Answered below.
4. Following the wearer by the pin's radio signature. Reduced below.
5. A malicious app on the phone forging what Spotify broadcasts. Answered in
   [android-app.md](../android-app.md).

## Decision

Proposed:

1. **Pairing with a passkey.** LE Secure Connections with passkey entry: the pin shows a
   random 6-digit passkey on its screen, the user types it on the phone. A fixed PIN is
   never used.
2. **Level 4 everywhere.** Every characteristic of the custom service, and the battery
   level, require an encrypted link from an authenticated LE Secure Connections pairing
   (security mode 1, level 4). A link paired without the passkey cannot read or write
   anything.
3. **Pairing window.** The passkey is only shown, and a new bond only kept, while the
   pairing window is open. The window opens with a button gesture, only when the pin has no
   bond. It closes after 60 s, at the first success, or after 3 failed attempts. A bond
   created outside the window, or below level 4, is deleted and the link is dropped.
4. **One bond.** The pin stores one bond. Once bonded, it drops any connection that is not
   encrypted with that bond within about 5 s.
5. **Erasing the bond.** A second button gesture, only when a bond exists, erases it and
   returns the pin to the unpaired state.
6. **No update over the air.** The firmware never starts the DFU service. The pin exposes
   only the generic services, the custom service and the battery level. Updates go through
   USB.
7. **Quiet advertising.** The pin's name and service UUID are advertised only during the
   pairing window. Once bonded, the advertising carries nothing that identifies the project.
   The device name cannot be written over the air.
8. **Private address.** A resolvable private address is tried together with Android's
   companion presence ([spike 0012](../spikes/0012-pairing-and-reconnection.md)). If the two
   do not work together, a fixed address is accepted.

Two refinements to try in the spike: the pin asking for encryption itself as soon as a phone
connects, instead of waiting 5 s; and an accept list based on the bonded phone's identity
key, which the Nordic stack offers below Bluefruit and which would spare the cost of
strangers connecting.

Proposed gestures, to check on the real buttons: with no bond, KEY2 held about 5 s opens the
pairing window; with a bond, KEY2 and KEY3 held together about 10 s erase it. The two
gestures cannot trigger each other because each only exists in one state.

## Accepted risks

- Jamming the link, or connecting repeatedly to drain the battery. Easy, low impact. The
  cost of dropping a stranger after 5 s is measured.
- Someone with the pin in hand can reflash it through USB, read its keys through the debug
  port, or erase the bond with the buttons. A lost pin is handled by removing the device on
  the phone.
- With a single pin in existence, its radio behaviour identifies it almost as well as its
  address.

## Alternatives

- Pairing without a passkey inside the pairing window. Rejected: it protects against
  listening but not against someone who interferes during pairing, and it leaves the whole
  guarantee to custom code, since the library accepts every pairing.
- A secret shared at the application level, on top of an open link. Rejected: it rebuilds
  what the Bluetooth security already provides.

## Consequences

- A stranger cannot write to the pin, read its battery level, or pair without seeing its
  screen.
- The pin needs ten digit glyphs and one more refresh, to show the passkey.
- Replacing the phone requires the erase gesture.
- The firmware needs an unpaired state with its own screens.
- A lost bond on either side has to be detected and explained by the app.
- Several of these points use the Nordic stack below Bluefruit. That is extra work, and it
  ties the firmware more closely to this chip.
