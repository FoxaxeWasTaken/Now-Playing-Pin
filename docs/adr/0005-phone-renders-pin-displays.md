# ADR-0005: The phone renders, the pin displays

- Status: Accepted
- Date: 2026-11-19 (recorded; the decision is older)
- Related: [architecture.md](../architecture.md), [android-app.md](../android-app.md),
  [spike 0006](../spikes/0006-app-preview-renderer.md)

## Context

A track title can be in any script and can contain emojis. A cover has to be cropped,
scaled, level-adjusted and dithered to 1 bit. The phone has fonts, an image pipeline, memory
and a network connection. The pin has a small microcontroller and a power budget.

## Decision

The phone prepares everything. The pin receives ready-to-draw data:

- the cover as a 122 × 122, 1-bit bitmap;
- the title and artist as a 128 × 122, 1-bit bitmap drawn by the phone;
- the share link (and later, the Spotify Code bar levels).

The pin holds no font and no image decoder.

## Alternatives

- Sending the title and artist as text and drawing them on the pin. Rejected: it needs fonts
  on the pin and cannot cover every script and emoji.

## Consequences

- Every script and emoji works with no font stored on the pin.
- The text costs 1,952 bytes per track instead of a few dozen bytes of text.
- The pin cannot lay the text out again by itself. Anything that changes the text area goes
  through the phone.
- The QR code is the exception: it is drawn on the pin from the link and not sent as a
  bitmap. The Spotify Code will follow the same rule.
- The pairing screen and the sleep image cannot come from the phone, so they are bitmaps
  built into the firmware.
- The app's preview shows the two bitmaps it sends. Showing exactly what the pin composes
  would need the layout on both sides.
