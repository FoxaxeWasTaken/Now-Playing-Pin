# Display specification

Status: **starting values.** They come from a design review on 2026-11-19, made on paper:
no mock-up has been drawn and nothing has been seen on a panel. They are precise so that the
rendering can be coded and tested with golden images, and they are expected to change after
[spike 0011](spikes/0011-legibility-at-scale.md).

## Screen

- 250 × 122 px, landscape, black and white, no grey.
- Active area 48.55 × 23.70 mm, about 0.194 mm per pixel (product page, **[to verify]**).
- Origin at the top left. A pixel is black or white; which bit value is black is fixed with
  the protocol byte layouts ([protocol.md](protocol.md)).
- The mounting side of the panel in the case is not known yet, so a 180° rotation is a
  build-time constant of the firmware from V1.

## Reading distance

The pin is read by someone else, at conversation distance.

| Element | Height | Readable up to about |
| --- | --- | --- |
| Title, capital letter 16 px | 3.1 mm | 1 m with attention, 0.65 m comfortably |
| Artist, capital letter 11 px | 2.2 mm | 0.7 m |
| Cover | 23.7 mm | Recognisable further away |

These distances are computed from visual acuity figures, not observed. Beyond one metre,
only the cover speaks. A wider text area would allow larger letters; the layouts are
compared in the spike before the text rendering is coded.

## Cover view

| Zone | x | y | Size | Drawn by |
| --- | --- | --- | --- | --- |
| Cover | 0 | 0 | 122 × 122 | Phone |
| Text | 122 | 0 | 128 × 122 | Phone |
| State badges | over the cover, bottom left | | 22 × 22 each | Pin |

### Text area

Rendered by the phone into the 128 × 122 bitmap.

- Margins: 8 px on the left, 4 px on the other sides. Usable area 116 × 114 px.
- Font: a bold condensed sans-serif under an open licence, **bundled with the app**, not the
  device's font. Stems at least 2 px wide. Not chosen yet. A script the bundled font does
  not cover (Arabic, Chinese, Japanese, Korean) falls back to the device's fonts.
- Rendering: drawn in grey with anti-aliasing, then cut at 50 %. No dithering on text.
- Title: 22 px font size, line height 25 px, 3 lines at most. If the title fits on 2 lines at
  26 px, it uses 26 px with a line height of 30 px.
- Artist: 16 px font size, bold, line height 18 px, 6 px below the title. One line if the
  title takes 3 lines, otherwise up to 2.
- The block made of the title and the artist is centred vertically in the usable area.
- A title that is too long: first remove what is in brackets or after " - " (featuring,
  remastered, live), then end with an ellipsis. The size is not reduced further.
- An artist that is too long ends with an ellipsis. An empty artist leaves the title alone,
  centred the same way. An empty title shows the artist in the title's place.
- Alignment follows the direction of the script: left for left-to-right, right for
  right-to-left.
- Chinese, Japanese and Korean text: at least 16 px.
- Emoji: a monochrome emoji font under an open licence, bundled with the app. A colour emoji
  is never thresholded.

### Cover

Processed by the phone:

1. Crop to a square, scale down to 122 × 122 by area averaging, in linear light.
2. Stretch the levels between the 2nd and the 98th percentile.
3. Lighten dark covers: gamma 0.8 when the median is under 0.35, on a scale from 0 (black)
   to 1 (white), measured after the stretch.
4. Light sharpening, 1 px radius at 50 %.
5. Atkinson dithering to 1 bit.

The constants are tuned on the reference covers below. Atkinson dithering gives strong
contrast and loses detail in very dark and very light areas, which is why steps 2 and 3
exist.

When a track has no cover, the phone draws a placeholder (a music note). The pin always
receives a cover image.

### State badges

Drawn by the pin over the bottom left corner of the cover. The phone reserves nothing.

- A badge is a white 22 × 22 px square with a 1 px black border, holding a 14 × 14 px glyph
  with strokes at least 2 px wide.
- At most two badges, side by side: first the state (disconnected takes priority over
  paused), then low battery.
- First badge: x from 2 to 23, y from 98 to 119. Second badge: x from 26 to 47, same y. A
  single badge takes the first place.
- The badges are for the wearer, who reads them upside down at about 40 cm. The glyphs are
  shapes that read the same either way up.
- No badge in the share view.

## Share view

| Zone | x | y | Size | Drawn by |
| --- | --- | --- | --- | --- |
| QR code | 12 | 11 | 99 × 99 | Pin |
| Text | 122 | 0 | 128 × 122 | Phone (same bitmap as the cover view) |

- QR version 4 (33 modules), 3 px per module, black on white, never inverted.
- The pin fills the whole 122 × 122 square with white before drawing the code.
- White margin: 12 px on the left (4 modules), 11 px at the top and 12 px at the bottom
  (just under 4 modules), 11 px on the right followed by the 8 px left margin of the text.
  The QR specification asks for 4 modules.
- Error correction level M when the link fits in 62 bytes (a Spotify track link is 53),
  level L up to 78 bytes. No larger version.
- No "scan me" pictogram: a QR code is recognised as such.

These QR settings are checked in [spike 0005](spikes/0005-qr-size-and-scannability.md).

## Built-in screens

Three bitmaps stored in the firmware. They may contain text: it is drawn when the firmware
is built, since the pin has no font at run time. The only thing drawn at run time is the
passkey, from ten digit glyphs also stored in the firmware.

| Screen | Shown when | Content |
| --- | --- | --- |
| Unpaired | The pin has no bond and the pairing window is closed | A drawing of the pin with an arrow to the pairing button, and how long to hold it |
| Pairing | The pairing window is open | The name the pin advertises, above the 6-digit passkey. No brand logo |
| Sleep | Before System OFF, and after a wake until the first bundle | A full-screen pattern with a power symbol. Nothing that goes out of date |

The passkey is drawn in six cells of 18 × 24 px, 4 px apart, with 10 px between the two
groups of three: 134 px in all, centred, from x = 58 to 191 and y = 72 to 95. Leading zeros
are shown. The golden images use 012345 and 678901, which cover the ten digits.

## Reference set

Used for the golden images and for tuning. All of it is made for the project: invented
titles and covers drawn for the purpose.

- Twelve titles: short, long, with brackets, with a "feat.", all capitals, accented letters,
  a right-to-left script, Chinese or Japanese, an emoji, a single very long word, an empty
  artist, a very long artist.
- Six covers: very dark, very light, high contrast, a photograph with fine detail, large flat
  areas, text on the cover.

## Not specified yet

- The glyphs of the badges, the ten digits and the three built-in bitmaps, to draw as PBM
  sources.
- The font, chosen in [spike 0011](spikes/0011-legibility-at-scale.md).
