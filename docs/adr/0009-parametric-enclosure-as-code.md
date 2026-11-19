# ADR-0009: Parametric enclosure as code

- Status: Accepted
- Date: 2026-11-19 (recorded; the decision is older)
- Related: [enclosure.md](../enclosure.md), [spike 0007](../spikes/0007-cad-tool.md)

## Context

The case has to be designed before any part can be measured. The dimensions available today
are published values, several of them **[to verify]**, and they will change once the parts
are in hand. The battery is not chosen yet either.

## Decision

The enclosure is modelled by code, with every dimension as a named variable at the top of
the file. The initial values are the published dimensions; measurements replace them later.

## Alternatives

- A graphical CAD tool. Rejected: the model would not live in the repository as readable
  text, and changing one dimension would be manual work.

## Consequences

- A corrected dimension is a one-line change, visible in the history.
- CI can render the model from the command line.
- The tool is not chosen yet: OpenSCAD or CadQuery.
- The size of the case is set in [ADR-0013](0013-enclosure-envelope.md).
