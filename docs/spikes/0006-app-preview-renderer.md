# Spike 0006: App preview renderer

- Status: Planned
- Needs the pin: no
- Time box: one day (suggested)
- Feeds: a future ADR, [testing.md](../testing.md)

Not needed for V1 ([ADR-0010](../adr/0010-v1-scope.md)). In V1 the preview shows the cover
bitmap and the text bitmap side by side, which is what the cover view contains apart from
the icons. This spike only matters once the preview has to show the share view or the icons.

## Question

How does the preview in the app show exactly what the pin will draw?

## Why it matters

The views are composed on the pin by the portable C++ renderer
([ADR-0008](../adr/0008-portable-renderer-and-simulator.md)). If the layout is written a
second time in the app, the two versions can drift apart.

## Options

- **A. Shared renderer.** Build `firmware/render` for Android and call it from the app. One
  implementation, but a native build inside the Android project, which also complicates the
  JVM unit tests.
- **B. Second implementation in Kotlin**, kept honest by comparing its output with the same
  golden images as the C++ renderer.

## Method

1. Prototype option A for the cover view: build, call from Kotlin, display.
2. Note what it costs: build setup, build time, effect on the JVM unit tests and on CI.
3. Estimate option B from the size of the renderer at that point.

## Done when

One option is chosen with its cost written down.

## Result

Not run.

## Outcome

To be written. Expected: an ADR, and the matching test level in testing.md.
