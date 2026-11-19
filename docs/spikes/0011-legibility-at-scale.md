# Spike 0011: Legibility at real size

- Status: Planned
- Needs the pin: no
- Time box: half a day (suggested)
- Feeds: [display-spec.md](../display-spec.md)

## Question

At what distance can someone who does not know the track read the title and the artist, and
which layout reads best: the cover with a narrow text column, or a wider text area?

## Why it matters

The pin exists to be read by someone else. A paper calculation says the planned title
(capital letters about 3 mm high) is readable up to about one metre and comfortable under
0.65 m. If that is not enough, the layout has to change, and it is far cheaper to learn it
from a printed mock-up than after the rendering is coded.

## Method

1. Pick two or three candidate fonts under an open licence. Draw 1-bit mock-ups at
   250 × 122 px for the titles of the reference set, in three variants: the planned layout at
   the planned sizes, the same layout one size larger, and a layout with the text across the
   full width. Draw the badge glyphs too.
2. Show them at real size (48.55 mm wide): printed, and on a phone screen at the matching
   zoom.
3. Pin them on a garment. Two people who do not know the titles read them at 0.5, 1, 1.5 and
   2 m.
4. Note for each variant and distance what was read correctly.
5. Look at the state badges upside down at 40 cm, as the wearer would.

Limit: white paper and a phone screen have more contrast than e-paper. The distances found
here are optimistic and are checked again on the real panel.

## Done when

- A layout and its text sizes are chosen, with the distance at which they were read.
- The font is chosen.
- The badge glyphs are drawn.
- The values in display-spec.md are confirmed or replaced.

## Result

Not run.

## Outcome

To be written. Expected: display-spec.md updated, and the mock-ups kept as the visual
reference that the simulator's golden images are compared with by eye.
