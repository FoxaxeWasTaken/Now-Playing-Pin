---
name: project-status
description: Give the maintainer a short status of the Now Playing Pin project and bring the "Current state" notes back in line with reality. Use when the maintainer asks where the project stands, what to do next, or comes back after a break.
---

# Project status

1. Read `docs/roadmap.md` (V1 conditions, milestones) and "Current state" in `CLAUDE.md`.
2. Look at the real state:

   ```bash
   git log --oneline -15
   gh issue list --state open --limit 50
   gh pr list
   gh issue list --state closed --limit 10
   ```

3. Read the indexes in `docs/adr/README.md` and `docs/spikes/README.md` for anything still
   `Proposed` or `Planned` that blocks the current milestone.
4. Run `python tools/check_docs.py`.
5. Tell the maintainer, in French and in a few lines:
   - the current milestone and what is left to reach it;
   - what was finished since the last status;
   - what is in progress, what is waiting (delivery, a measurement, a print) and for what;
   - the next ticket that is ready, preferably a small one;
   - anything that drifted: a ticket outside the V1 scope, a decision still proposed, a
     document that no longer matches.
6. If "Current state" in `CLAUDE.md` or the status note in `README.md` is no longer true,
   propose the corrected text. Edit the files only when the maintainer agrees.

Do not create, close or move tickets from this skill. Report, and let the maintainer decide.
