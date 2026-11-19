# ADR-0001: Record decisions as ADRs and open questions as spikes

- Status: Accepted
- Date: 2026-11-19
- Related: [spikes](../spikes/README.md)

## Context

The project is built by one person, in spare time, with long gaps between sessions. The
hardware is not available yet, so many choices rest on public datasheets and estimates. Two
things get lost easily in that situation: why a choice was made, and which choices are still
waiting for evidence.

## Decision

- Every decision that shapes the project is written as an ADR in `docs/adr/`.
- Every question that needs an experiment before a decision is written as a spike in
  `docs/spikes/`: one question, a method, a time box, an exit condition.
- A spike ends with a recorded result, including when the answer is negative. If the result
  leads to a decision or changes one, a new ADR is written.
- An accepted ADR is not edited. A new ADR supersedes it, and only the status line of the
  old one changes.

## Alternatives

- Keeping decisions in the README and the design documents only. They describe the current
  design well but not what was rejected or why.

## Consequences

- The design documents describe the system as it is; the ADRs explain why.
- Several decisions recorded on 2026-11-19 were made before this log existed. They say so
  next to their date.
- An accepted decision can only change through a new ADR, which makes reversals visible.
- Not every choice needs an ADR. A choice that is cheap to reverse is recorded in the pull
  request that makes it.
