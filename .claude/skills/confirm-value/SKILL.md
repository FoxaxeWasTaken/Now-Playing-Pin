---
name: confirm-value
description: Replace an unverified value of the Now Playing Pin project (tagged [to verify], or listed as "not measured") with a checked one, everywhere it appears. Use when the maintainer reports a single value checked on the real part outside a spike - a caliper reading, a pinout, a polarity, a current.
argument-hint: <value and how it was checked>
---

# Confirm a value

Report: $ARGUMENTS

1. Make sure the evidence is there: what was checked, how (instrument, part in hand), and
   when. If the "how" is missing, ask. A value read on a web page is a source, not a check:
   it is recorded with its source and keeps its tag. The tag only goes when the value was
   checked on the real part.
2. Find every place the value appears: search the repository for the old value, for the tag
   `[to verify]` near it, and for the names it goes by. Include `docs/`, `README.md`,
   `CLAUDE.md`, the enclosure parameters and any constant in code.
3. Update each place with the checked value and remove the tag. If the new value differs
   from the published one, keep both where it helps: "27.3 mm measured (27 mm published)".
4. Record the check where measurements live:
   - dimensions and pins: `docs/hardware.md`;
   - currents: the measurement table in `docs/power.md`.

   With the date, the instrument and the conditions.
5. Follow the consequences and list them for the maintainer:
   - a dimension changes the enclosure parameters;
   - a pin changes the firmware configuration;
   - a current changes the battery life estimate, which stays labelled as an estimate until
     the average is measured;
   - a value that contradicts an accepted ADR needs a new ADR.
6. An ADR's text is not edited. If it quotes the old value, leave it: the design documents
   carry the current one.
7. Run `python tools/check_docs.py`. It prints the remaining tags per file.
8. The pull request says how the value was checked.
