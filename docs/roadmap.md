# Roadmap

## V1

V1 is the smallest pin worth wearing. Its scope is fixed by
[ADR-0010](adr/0010-v1-scope.md): the cover view, the QR share view and a measured battery
life. The user is the maintainer.

V1 is done when:

1. The pin shows the cover, title and artist of the track playing on the phone, and follows
   track changes with no action from the user.
2. A press on KEY1 shows a QR code that a phone camera opens as a link to the track.
3. The pin reconnects by itself after being out of range and after the phone restarts.
4. The pin is in its printed case and has been worn for a full day.
5. The pin goes to sleep and wakes with its buttons.
6. The average current has been measured, and the battery life computed from it is written
   in [power.md](power.md).

## Milestones

Each milestone ends with something that can be shown.

| Milestone | Reached when |
| --- | --- |
| M1 Simulated pin | A real track playing on the phone is rendered as an image by the PC simulator |
| M2 Pin on the desk | The real pin shows the track playing on the phone |
| M3 Wearable V1 | The V1 conditions above are met |
| Later | Everything else |

## Hardware: start now

The riskiest questions (board idle current, display driver, connection interval) can only
be answered with the real parts, and delivery takes time. Ordering does not wait for the
software. These tickets belong to milestone M2.

1. Close the checks listed under "To check before ordering" in [hardware.md](hardware.md).
2. Order the board, the panel and a test battery: any protected cell of 100 mAh or more with
   the right connector is enough for the spikes. A spare panel is worth considering: the
   glass is fragile and every hardware test depends on it.
3. Make sure an instrument that measures microamperes is at hand (a suitable multimeter or a
   power profiler): the first spike on reception needs it.
4. The final battery and the magnet clip are ordered later, once the parts are measured and
   the place of the antenna is known.

## M1 Simulated pin

Nothing here needs the pin.

1. **The whole CI first.** Build and test scaffolding on both sides (PC build of the portable
   C++, Android project with JVM tests), the code quality tools listed in
   [testing.md](testing.md), and one CI job per part. Nothing else starts before it is green.
2. Protocol draft v0: [protocol.md](protocol.md) with byte layouts for everything that does
   not depend on an experiment. The protocol is frozen later, in M2.
3. Format of the shared test vectors and the first vectors.
4. Packet encoder in Kotlin and decoder in C++, both against the shared vectors.
5. App: listen to MediaSession and to the Spotify broadcast, and log the events. This code is
   kept. [Spike 0002](spikes/0002-metadata-event-timing.md) is the measurement campaign run
   with it.
6. [Spike 0011](spikes/0011-legibility-at-scale.md): legibility at real size, on printed
   mock-ups. Then the app's cover conversion and text rendering, following
   [display-spec.md](display-spec.md), with a preview that shows the two bitmaps side by side.
7. Firmware, in the portable code and rendered by the simulator: framebuffer, cover view,
   state badges, built-in screens, state machine and bundle reassembly.
8. Pipeline test: a bundle encoded by the app code is rendered by the simulator.
9. Export from the phone: in a debug build, the app writes the bundle of the track that is
   playing to a file, and the simulator renders it. This is the demo that ends M1.
10. [Spike 0005](spikes/0005-qr-size-and-scannability.md), first part, on a monitor: it
    fixes the length limit of the share link, which the protocol needs before it is frozen.

## M2 Pin on the desk

On reception, in this order:

1. Hardware checks listed in [hardware.md](hardware.md).
2. [Spike 0008](spikes/0008-board-idle-current.md): idle current of the board alone.
3. [Spike 0009](spikes/0009-display-driver.md): driving the panel from the EN05, with partial
   refresh. It ends with a first thing to show: an image from the simulator on the real
   panel.
4. [Spike 0012](spikes/0012-pairing-and-reconnection.md): passkey pairing and automatic
   reconnection. [ADR-0012](adr/0012-ble-security.md) is accepted or replaced after it.
5. [Spike 0004](spikes/0004-connection-interval-android.md): connection parameters with an
   Android phone.

Then:

1. App: BLE layer (association, GATT client, transfer, foreground service, reconnection).
   [Spike 0003](spikes/0003-ble-bundle-transfer.md) comes first.
2. Firmware on the hardware: display, GATT server, security, sleep.
3. Protocol frozen, once spikes 0003, 0004, 0005 (first part) and 0012 have answered its
   open questions.

## M3 Wearable V1

1. QR share view, and the second part of
   [spike 0005](spikes/0005-qr-size-and-scannability.md) on the real panel.
2. [Spike 0010](spikes/0010-partial-refresh-ghosting.md): partial refresh on dithered covers.
3. Enclosure: [spike 0007](spikes/0007-cad-tool.md), then the case modelled from the measured
   parts.
4. Power measurements ([power.md](power.md)) and the battery life estimate.
5. A full day worn.

## Later

Not in V1. None of this is started before V1 is done.

- Spotify Code share view ([spike 0001](spikes/0001-spotify-code-endpoint.md)).
- Media control from the pin: play/pause and next on KEY2 and KEY3.
- Settings: rotation, inversion, discreet mode, custom sleep image, choice of apps.
- App preview identical to the pin for every view
  ([spike 0006](spikes/0006-app-preview-renderer.md)).
- History.

### V2: NFC "tap to play"

The pin emulates an NFC tag that carries the share link.

- Active **only while the share view is shown**: started when the view is toggled, stopped on
  return to the cover view or on sleep, reloaded if the track changes.
- Automatic return to the cover view after 30 to 60 s.
- Known trap: handle FIELDLOST and go back to SENSE, otherwise the chip may stay at about
  3 mA.
- The board may have no NFC antenna **[to verify]** (see [hardware.md](hardware.md)). V2
  would then need an external coil, and the case reserves room for it from V1.

No V2 code is written until V1 runs on real hardware.

## Risks

| Risk | Effect | Answer |
| --- | --- | --- |
| The board's idle current is higher than assumed | The battery target is missed whatever the firmware does | Spike 0008, first thing on reception |
| Android does not grant a low-power connection | Same | Spike 0004 |
| The display library does not drive this panel on the EN05, or not in partial refresh | A display driver has to be written | Spike 0009 |
| The Spotify Code endpoint changes or disappears | The Spotify Code is lost, nothing else | Kept out of V1, QR code is the share view |
| There is one pin and its glass breaks | Hardware work stops until a new panel arrives | Spare panel, case tested before wearing |
| Months of work with nothing real to show | Loss of motivation | Milestones that end with a demo, hardware ordered early |
| The battery lies over the BLE antenna and weakens the link | A shorter cell with less capacity | Range measured with the cell in place, before the final cell is ordered |
| The scope grows | V1 never ends | ADR-0010, and the "Later" list above |
| The title cannot be read at a useful distance | The pin does not do its job | Spike 0011, before the rendering is coded |
| The Bluetooth library cannot enforce the security rules | More firmware work below the library, or weaker rules | Spike 0012 |
