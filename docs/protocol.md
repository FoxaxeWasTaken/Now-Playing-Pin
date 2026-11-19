# BLE protocol (draft v0)

Status: **draft, not frozen.** Nothing is implemented. The byte layouts are written in step 1
of the [roadmap](roadmap.md). The protocol is frozen in M2, once the spikes listed at the end
have answered its open questions.

This document covers V1 ([ADR-0010](adr/0010-v1-scope.md)). What is planned for later is
listed so that the format leaves room for it.

## Roles

- Pin: GATT peripheral (server).
- Phone: central (client).

## Version

The protocol carries a version number. The pin announces the version it speaks when the
phone subscribes (see "Connection lifecycle"), and every bundle starts with the version the
phone uses. A side that receives a version it does not know refuses the bundle and says so
on Control. The app and the firmware are released by the same person, so V1 supports exactly
one version at a time.

## Security

Proposed in [ADR-0012](adr/0012-ble-security.md), not accepted yet.

- Pairing uses LE Secure Connections with a passkey: the pin shows a random 6-digit passkey
  on its screen and the user types it on the phone.
- Every characteristic of the custom service, and the battery level, require security mode 1
  level 4: a link encrypted from an authenticated pairing. A device paired any other way can
  connect but cannot read, write or subscribe.
- The passkey is only shown, and a new bond only kept, during a pairing window opened by a
  button gesture, and only when the pin has no bond. The window lasts 60 s.
- The pin keeps one bond. Once bonded, it drops any connection that is not encrypted with
  that bond within about 5 s.
- A second button gesture erases the bond.
- The pin's name and service UUID are advertised only during the pairing window.
- No firmware update over the air. The pin exposes no update service.

Open, for [spike 0012](spikes/0012-pairing-and-reconnection.md): whether the Bluetooth
library behaves as the ADR assumes, and whether the pin can use a private address without
breaking automatic reconnection.

## Services

- One custom service, 128-bit UUID (not generated yet).
- The battery level, as the standard characteristic (0x2A19) of the standard Battery Service
  (0x180F), but protected at level 4 like the rest. The library's ready-made battery service
  is not used, because it is readable without encryption **[to verify]**.
- Nothing else beyond the two generic services every BLE device has. The device name cannot
  be written over the air.

## Characteristics of the custom service

| Characteristic | Direction | Property | In V1 | Content |
| --- | --- | --- | --- | --- |
| Track | phone → pin | Write | yes | Opens a bundle: protocol version, track counter, playback state, size of each image, share link (may be empty) |
| Cover | phone → pin | Write Without Response | yes | 1 bit per pixel, rows padded to a byte. In V1: 122 × 122, so 16 × 122 = 1,952 bytes |
| Text | phone → pin | Write Without Response | yes | 1 bit per pixel, rows padded to a byte. In V1: 128 × 122, so 16 × 122 = 1,952 bytes |
| State | phone → pin | Write | yes | Playback state alone, when it changes without a track change |
| Control | pin → phone | Notify | yes | Hello, acknowledgement, resend request, refusal |
| SpotifyCode | phone → pin | Write | later | 20 levels × 3 bits, packed in 8 bytes |

State as a separate characteristic is a proposal. The alternative is a message type on Track.

Each image announces its width and height in Track. The split between cover and text
(122 and 128 px) is a choice of the layout ([display-spec.md](display-spec.md)), not a
constant of the protocol, so the layout can change without a new protocol version. The pin
refuses sizes it cannot place.

Planned for later and not designed: button events on Control (media control), settings, a
custom sleep image.

## Sizes

- The MTU is negotiated, never assumed. The pin offers up to 247 **[to verify]** and the
  phone uses the value it is given. A packet carries the negotiated MTU minus 3 bytes.
- At an MTU of 247, each image is 1,952 bytes in about 9 writes, and a bundle is about 20
  writes.
- The share link is 78 bytes at most. That is what a version 4 QR code holds at the lowest
  error correction level, and version 4 is the largest code that keeps a usable margin on
  this screen ([display-spec.md](display-spec.md)). Confirmed or lowered by
  [spike 0005](spikes/0005-qr-size-and-scannability.md).

## Connection lifecycle

```mermaid
sequenceDiagram
    participant Phone
    participant Pin
    Note over Phone,Pin: Link encrypted with the bond, MTU negotiated
    Phone->>Pin: Subscribe to Control
    Pin-->>Phone: Hello (versions, identity of the bundle held)
    alt The pin holds another bundle, or none
        Phone->>Pin: Track (opens the bundle)
        Phone->>Pin: Cover packets
        Phone->>Pin: Text packets
        Phone->>Pin: End of transfer (lengths, CRC16)
        Pin-->>Phone: Acknowledgement, or resend request
        Note over Pin: One screen refresh
    end
    Phone->>Pin: State (playing, paused, nothing playing)
```

The same exchange, without the hello, happens on every track change.

1. **Unpaired.** A button gesture opens the pairing window and the pin advertises its name.
   The phone associates and asks for the bond. Only then does the pin generate a passkey and
   show it; the user types it on the phone.
2. **Connection.** The phone connects, the link is encrypted, the MTU is negotiated, the
   phone discovers the services and subscribes to Control. The phone writes the subscription
   again on every connection, even though a bonded device may have it restored for it: that
   write is what triggers the hello.
3. **Hello.** When the phone subscribes, the pin notifies: its protocol version, its firmware
   version and the identity of the bundle it holds, or "none". The identity is a checksum of
   the bundle's content only (the two images and the link), without the track counter or the
   playback state, so that it stays the same across restarts. A 32-bit checksum is proposed,
   to make a false match unlikely.
4. **Synchronisation.** The phone compares that identity with the bundle for the current
   track. If they differ, it sends the bundle. In every case it then sends State, because
   playback may have changed while the pin was out of reach.
5. **Running.** Bundles on track changes, State on playback changes.
6. **Link lost.** The pin keeps its bundle and advertises slowly. The phone reconnects by
   itself. Steps 2 to 4 run again.
7. **Wake from sleep.** The bundle was in RAM and is gone. The pin keeps the sleep image on
   screen and announces "none" in its hello. The first bundle replaces the sleep image.

The same sequence covers a restart of the app, a restart of the phone and a restart of the
pin: none of them needs a special case.

## Transfer of one bundle

1. The phone writes Track. This opens a bundle and cancels any bundle in progress.
2. It writes the Cover and Text packets, one write at a time: it waits for the write callback
   before sending the next packet.
3. End of transfer: the phone sends the lengths and a CRC16.
4. The pin answers on Control with an acknowledgement or a resend request. In V1 a resend
   request means the whole bundle.
5. The pin refreshes the screen **once**, when the bundle is complete and checked.

Rules:

- Every image packet carries the track counter and an offset.
- The pin compares the track counter by equality with the counter of the bundle in progress.
  A packet with any other value is dropped. The counter is not ordered, so a restart on
  either side cannot confuse it.
- A bundle always carries both images. The link may be empty: the pin then has no share
  view for that track.

## Playback state

The phone writes State when playback changes and the track does not: playing, paused, or
nothing playing. The pin updates its icon ([firmware.md](firmware.md)). The phone only sends
a State that differs from the last one it sent.

## Connection parameters

Not decided. Two options are compared in
[spike 0004](spikes/0004-connection-interval-android.md):

- **A.** A long interval (500 ms to 1 s) outside transfers, and
  `CONNECTION_PRIORITY_HIGH` requested by the phone during a transfer. Risk: a change of
  parameters takes several connection events to apply, which at a long interval may last
  longer than the transfer itself.
- **B.** An interval of about 100 ms with slave latency, which the pin stops using while a
  transfer is in progress. No parameter change is needed around a transfer.

In both cases the pin requests the parameters it wants. A priority request from the phone
replaces what the pin obtained, so the pin has to ask again afterwards. Android's own
`LOW_POWER` preset is reported as a 100 to 125 ms interval, not 500 ms to 1 s **[to verify]**.

## Open questions

Settled in step 1, by writing them down:

- UUIDs of the service and of each characteristic.
- Byte layout of Track, State and the Control messages.
- Header of an image packet: width of the offset field, total header size.
- Bit order inside a byte and polarity (which value is black), chosen with the panel's own
  memory layout in mind ([firmware.md](firmware.md)).
- Where the end-of-transfer record goes, what the CRC16 covers, and which CRC16 variant.
- Smallest MTU the protocol accepts.
- Timeouts: when the pin gives up on an incomplete bundle, and when the phone gives up on a
  missing acknowledgement.
- Search link: which site it points to, and how it is shortened to fit the limit when the
  title is long or not in Latin script.

Waiting for an experiment:

- Packet pacing and transfer time: [spike 0003](spikes/0003-ble-bundle-transfer.md).
- Connection parameters: [spike 0004](spikes/0004-connection-interval-android.md).
- Link length limit: [spike 0005](spikes/0005-qr-size-and-scannability.md).
- Security as proposed in [ADR-0012](adr/0012-ble-security.md), and the content of the
  advertising: [spike 0012](spikes/0012-pairing-and-reconnection.md).
