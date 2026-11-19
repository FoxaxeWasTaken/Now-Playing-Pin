# Spike 0010: Partial refresh on dithered covers

- Status: Planned
- Needs the pin: yes
- Time box: half a day (suggested)
- Feeds: [firmware.md](../firmware.md), [power.md](../power.md)

Depends on [spike 0009](0009-display-driver.md).

## Question

Is a partial refresh good enough to replace one dithered cover with another, and how many
partial refreshes can follow each other before a full one is needed?

## Why it matters

The refresh policy assumes a partial refresh per track and a full one about every 10
updates. A dithered cover changes almost every pixel, which is the worst case for ghosting.
The fallback, a full refresh on every track, is estimated at about 15 µA more on average:
affordable, but slower and with a visible flash.

## Method

1. Display a series of real bundles, partial refresh only, and photograph the screen after
   1, 5, 10 and 20 updates.
2. Compare with the simulator image of the same bundle. Judge the cover and the text area
   separately: a ghost in a dithered cover looks like noise, while the ghost of the previous
   title on a white background is plainly visible.
3. Find the number of partial refreshes after which the image is no longer acceptable.
4. Measure the time and the current of a partial and of a full refresh.
5. Repeat the series once in the cold, near the low end of the panel's range.

## Done when

- The number of partial refreshes between two full ones is chosen, possibly zero. One
  possible outcome: a full refresh for each track, and partial refreshes only for the badges
  and the view toggle.
- The cost of both kinds of refresh is measured.

## Result

Not run.

## Outcome

To be written. Expected: the refresh policy in firmware.md fixed, and two rows of the
measurement table in power.md.
