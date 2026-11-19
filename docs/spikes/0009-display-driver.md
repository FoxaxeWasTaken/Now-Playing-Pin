# Spike 0009: Driving the panel from the EN05

- Status: Planned
- Needs the pin: yes
- Time box: one day (suggested)
- Feeds: [firmware.md](../firmware.md), [hardware.md](../hardware.md)

## Question

Does Seeed's display library drive the 2.13" black and white panel on the EN05, including
partial refresh, from a framebuffer the project provides?

## Why it matters

The firmware only needs one thing from the display library: push a 1-bit framebuffer to the
panel, fully or partially, then put the panel to sleep. Three points are uncertain:

- the configuration for this board with this panel is not given on the wiki;
- a forum thread reports a build failure for this combination with the older library;
- the wiki examples only show full refresh.

If the library does not do the job, a small driver for the SSD1680 controller has to be
written, and it is better to know early.

## Method

1. Build the vendor's example for this panel with the library the wiki names. Note the exact
   configuration that works.
2. Display a full-screen test bitmap from a framebuffer in the project's layout (landscape,
   250 × 122). Note the conversion needed to the panel's memory layout.
3. Try a partial refresh of the whole screen, then of one half.
4. Put the panel to sleep and wake it. Then try a partial refresh after the sleep, after the
   display supply was cut, and after a restart of the pin: a partial refresh relies on the
   previous image, which the controller may have lost.
5. If any step fails: estimate the work for a minimal SSD1680 driver from its datasheet.

## Done when

- The working configuration is written in hardware.md, or the decision to write a driver is
  recorded.
- Full refresh, partial refresh and sleep each have a yes or a no.

## Result

Not run.

## Outcome

To be written. Expected: the toolchain section of hardware.md completed, the display driver
section of firmware.md updated, and an ADR if a driver is written.
