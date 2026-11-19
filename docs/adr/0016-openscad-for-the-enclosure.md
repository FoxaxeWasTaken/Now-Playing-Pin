# ADR-0016: OpenSCAD for the enclosure

- Status: Accepted
- Date: 2026-11-20
- Related: [ADR-0009](0009-parametric-enclosure-as-code.md),
  [spike 0007](../spikes/0007-cad-tool.md), [enclosure.md](../enclosure.md),
  [testing.md](../testing.md)

## Context

The enclosure is modelled by code ([ADR-0009](0009-parametric-enclosure-as-code.md)). Two
tools were considered: OpenSCAD and CadQuery. Spike 0007 was planned to model the same test
piece in both. It was not run.

What was known when choosing:

- The maintainer does not know CadQuery.
- CI is to stay as cheap as possible. OpenSCAD is one package of the CI runner's Ubuntu
  release. CadQuery is a Python library built on the OpenCascade kernel, installed with its
  binding to that kernel. The install time of each in CI was not measured.
- The last stable release of OpenSCAD is 2021.01. A 2026.11 release candidate was published
  on 2026-11-07.

## Decision

The enclosure is modelled in OpenSCAD 2021.01. CI exports every model to STL, with warnings
treated as errors.

## Alternatives

- CadQuery. Not evaluated: spike 0007 was not run. Set aside because the maintainer does not
  know it, and because a Python library with a CAD kernel is expected to cost more in CI than
  one system package. That second reason is an expectation, not a measurement.

## Consequences

- The parameters are plain variables at the top of each file, as ADR-0009 asks.
- OpenSCAD has no fillet or chamfer operation. Rounded edges and the thin flexures of the
  buttons are built from `hull()`, `minkowski()` or `offset()`: more work, and slower to
  render.
- OpenSCAD exports meshes (STL, 3MF), not STEP. A 3D printer needs nothing more.
- The board outline from the Seeed PCB files is used as DXF or SVG, if the PCB tool exports
  one, or copied in as numbers.
- Moving to a newer release, once there is one, is a change to the CI job and to the local
  install, made in a pull request.
- No effect on the pin's consumption.
