# Spike 0007: OpenSCAD or CadQuery

- Status: Abandoned
- Needs the pin: no
- Time box: half a day (suggested)
- Feeds: a future ADR, [enclosure.md](../enclosure.md),
  [ADR-0009](../adr/0009-parametric-enclosure-as-code.md)

## Question

Which tool is used to model the enclosure: OpenSCAD or CadQuery?

## Why it matters

The enclosure is parametric and written as code. The tool decides how the parameters are
expressed, how easy rounded edges and thin flexible parts are, and how the model is built in
CI.

## Method

Model the same small test piece in both tools: one corner of the case with the screen
window, a clip and one button flexure.

Compare:

- how the parameters at the top of the file read;
- fillets and chamfers;
- use of the Seeed PCB files as a reference for the board outline;
- command-line export for CI;
- installation on Windows;
- time spent to reach the same result.

## Done when

One tool is chosen and the test piece exists in it.

## Result

Not run.

## Outcome

Abandoned on 2026-11-20 without being run: the maintainer chose OpenSCAD, because they do
not know CadQuery and want CI kept cheap ([ADR-0016](../adr/0016-openscad-for-the-enclosure.md)). The test
piece was not modelled in either tool, so the comparison above was never made.
