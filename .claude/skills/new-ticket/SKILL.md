---
name: new-ticket
description: Create a GitHub issue for the Now Playing Pin project with the right template, labels and milestone, and add it to the project board. Run by the maintainer with /new-ticket.
argument-hint: <what the ticket is about>
disable-model-invocation: true
---

# Create a ticket

Subject: $ARGUMENTS

## Before writing

1. Read the documents the work depends on (table in `CLAUDE.md`). A ticket links to them, it
   does not copy them.
2. Check the subject is inside the V1 scope (`docs/adr/0010-v1-scope.md`). If it is not, say
   so: it goes to the `Later` milestone or is not created.
3. Search for an existing issue: `gh issue list --state all --search "<keywords>"`. If one
   covers the subject, say so and stop.
4. Pick the template in `.github/ISSUE_TEMPLATE/`: `task.md`, `spike.md` or `bug.md`. Keep
   its sections and their order.

## Content rules

- English, like everything in the repository.
- Title: `<area>: <imperative sentence>`, with the same areas as commits (`docs`, `android`,
  `firmware`, `enclosure`, `hardware`, `tools`, `ci`).
- One ticket is one pull request. If "Done when" needs more than about five boxes, split it.
- "Done when" lists things that can be checked, not intentions.
- "Depends on" names the tickets that must be closed first. "Out of scope" says what the
  ticket does not do.
- Never state a **[to verify]** value as a fact, and never invent a pinout, a UUID or a
  dimension. If the work depends on such a value, say which one.
- A spike ticket points to a file in `docs/spikes/`. If the file does not exist, create it
  with the `spike` skill first.

## Labels, milestone and board

The lists are in `CONTRIBUTING.md` ("Labels and milestones"). Use them as written there:

- one `area:*` label and one `type:*` label;
- `needs-hardware` if it cannot be finished without the pin;
- `power` if it can change the pin's consumption;
- one milestone.

Do not create a new label or milestone without asking.

## Create

Show the title, labels, milestone and body to the maintainer first, unless they asked for the
ticket to be created directly. Then:

```bash
gh issue create --title "<title>" --body-file <file> \
  --label "<labels>" --milestone "<milestone>" --project "Now Playing Pin"
```

Write the body to a temporary file outside the repository. Report the issue number and URL.
