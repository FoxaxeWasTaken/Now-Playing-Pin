---
name: start-ticket
description: Start work on a GitHub issue of the Now Playing Pin project - read the ticket and the documents it links, create the feature branch, and mark the ticket as in progress. Run by the maintainer with /start-ticket.
argument-hint: <issue number>
disable-model-invocation: true
---

# Start a ticket

Issue: $ARGUMENTS

1. Read it: `gh issue view <number> --comments`.
2. Read every document, ADR and spike it links, and the matching part of `docs/testing.md`.
   If the ticket conflicts with an accepted ADR, is outside the V1 scope, depends on an open
   ticket, or needs a **[to verify]** value to be true, stop and say so before touching
   anything.
3. Check the working tree is clean (`git status`) and that no other ticket is in progress.
   If either is not the case, stop and ask.
4. Create the branch from an up-to-date `main`, linked to the issue:

   ```bash
   git switch main && git pull --ff-only
   gh issue develop <number> --base main --name <area>/<short-description> --checkout
   ```

   The area is the one in the ticket title. The description is two to four words in
   kebab-case. This command creates the branch on GitHub as well as locally: running this
   skill is the maintainer's request for it.
5. Assign the ticket and move it to "In Progress" on the board:

   ```bash
   gh issue edit <number> --add-assignee @me
   gh project list --owner @me    # find the number of "Now Playing Pin"
   gh project item-edit <project number> --owner @me --url <issue url> \
     --field Status --value "In Progress"
   ```

   The value must match the column name on the board exactly. If the command refuses it,
   read the names with `gh project field-list <project number> --owner @me`.

6. Tell the maintainer, in French: what the ticket asks, which documents apply, the "Done
   when" list, which tests are expected, and anything unclear. Then start the work.

Do not commit or push the work unless asked. When the work is finished, the maintainer runs
`/open-pr`.
