---
name: new-adr
description: Record an architecture decision for the Now Playing Pin project as a numbered ADR in docs/adr, or supersede an existing one. Use when the maintainer makes or asks to document a choice that is costly to reverse - a framework, a protocol or security rule, a change of scope or of direction.
argument-hint: <the decision>
---

# Write an ADR

Decision: $ARGUMENTS

1. Read `docs/adr/README.md` (rules, template, index) and the ADRs related to the subject.
2. Check it deserves an ADR: a choice that is cheap to reverse (a library, a file format, a
   test framework) is recorded in the pull request that makes it, not here. If in doubt, ask.
3. Decide what this is:
   - a new decision: next free number;
   - a change to an accepted decision: a new ADR that supersedes the old one. The old file
     only gets its status line changed to `Superseded by ADR-NNNN`. Its text is not edited.
4. If the decision rests on something that has not been tried or measured, it is not ready:
   propose a spike instead, or write the ADR as `Proposed`.
5. Write `docs/adr/NNNN-short-title.md` from the template.
   - Context: facts and constraints. Say what is an estimate and what is **[to verify]**.
   - Decision: one or two sentences.
   - Alternatives: only those that were really considered, with the real reason each was
     set aside. If an alternative was not evaluated, write that. Do not invent a
     justification.
   - Consequences: what gets easier, what gets harder, what to watch. Mention any effect on
     the pin's consumption.
6. Status is `Proposed` until the maintainer says the decision is taken. Only then
   `Accepted`.
7. Add the line to the index in `docs/adr/README.md`, with the same status.
8. Update the design documents that describe the affected part so they match the decision,
   and link the ADR from them.
9. Run `python tools/check_docs.py`.
10. Tell the maintainer, in French, what was recorded and which documents changed.
