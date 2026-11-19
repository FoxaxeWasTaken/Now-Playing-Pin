# Spike 0005: QR code size and scannability

- Status: Planned
- Needs the pin: partly (first part on a monitor, confirmation on the real panel)
- Time box: half a day for the first part (suggested)
- Feeds: [display-spec.md](../display-spec.md), [protocol.md](../protocol.md)

## Question

Is a version 4 QR code at 3 px per module, with a margin slightly under the recommended one,
read reliably by an ordinary phone camera on this screen?

## Why it matters

The share view depends on it, and so does the length limit of the share link.

Known from the QR specification:

| Version | Modules | Size at 3 px | Bytes at level L | Bytes at level M |
| --- | --- | --- | --- | --- |
| 4 | 33 | 99 px | 78 | 62 |
| 5 | 37 | 111 px | 106 | 84 |

A version 5 code in the 122 px square leaves about 5 px of margin, under two modules when
four are asked for. The starting choice is therefore version 4 only: level M for a Spotify
link (53 bytes), level L up to 78 bytes, and a link limit of 78 bytes. The margin is then
about 3.8 modules.

## Method

1. Generate the codes: a Spotify link at level M, a 78-byte link at level L. The simulator
   is used if it already draws the share view; otherwise any QR tool set to the same version
   and level, drawn at 3 px per module. This first part does not wait for the firmware: the
   protocol's link limit depends on it.
2. Show them on a monitor at the physical size of the panel (active area 48.55 mm wide for
   250 px), inside a black frame that stands for a dark case touching the margin, and scan
   them with several phones, from the distance at which someone would lean towards the pin.
3. If a code is not read reliably: try a white border drawn into the text area, then a
   shorter link limit.
4. On reception, repeat on the real panel, indoors and outdoors.

## Done when

- The version, the error correction levels and the margin are confirmed or changed.
- The link limit in the protocol is confirmed or lowered.

## Result

Not run.

## Outcome

To be written. Expected: fixed values in display-spec.md and protocol.md, and QR golden
images.
