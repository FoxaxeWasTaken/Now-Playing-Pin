---
name: new-ticket
description: Write a GitHub issue for the Now Playing Pin project (task, spike or bug) and put it on the board with its milestone, labels, priority, size, status and blockers. Run by the maintainer with /new-ticket.
argument-hint: <what the ticket is about>
disable-model-invocation: true
---

# New ticket

Subject: $ARGUMENTS

The anatomy, the labels, the fields and the Definition of Ready are in `CONTRIBUTING.md` §7;
the templates are in `.github/ISSUE_TEMPLATE/`. This skill only orders the steps.

1. **Check it does not exist**: `gh issue list --search "<words>" --state all`. Check it is
   inside the V1 scope (`docs/adr/0010-v1-scope.md`); if not, it goes to the `Later`
   milestone or is not created.
2. **Pick the milestone** from `docs/roadmap.md`. Every ticket has one.
3. **Write the body** from the template, every section filled for an agent who will not ask:
   exact paths, a checkable *Done when* (a command, a test, a threshold), a binding *Out of
   scope*. Link the documents, ADRs and spikes instead of copying them. Never state a
   **[to verify]** value as a fact. Title: `<area>: <imperative sentence>`.
4. **Show the draft to the maintainer** before creating anything: title, milestone, labels,
   fields, blockers. Skip this only if they asked for the ticket to be created directly.
5. **Create**: `gh issue create --title … --body-file … --milestone "<title>" --label task
   --label area:<…>`, with the body in a temporary file outside the repository. Add
   `needs-hardware` or `power` when they apply.
6. **Board**: `gh project item-add <number> --owner <owner> --url <issue url>`, then one call
   per field (Status, Priority, Size, Who):
   `gh project item-edit <number> --owner <owner> --url <issue url> --field <field> --value "<option>"`.
   The option names are read with `gh project field-list <number> --owner <owner>`.
7. **Blockers**, with GitHub's native relationship, never a label: GraphQL `addBlockedBy`
   with the node ids from `gh issue view <n> --json id`. Status **Backlog** while a blocker
   is open, **Ready** only when the Definition of Ready holds.

A spike ticket points to a file in `docs/spikes/`: create that file with the `spike` skill
first if it does not exist. Do not create a label or a milestone without asking.
