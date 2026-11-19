---
name: start-ticket
description: Start work on a GitHub issue of the Now Playing Pin project - read the ticket and the documents it links, create the feature branch, and move the ticket to In Progress. Run by the maintainer with /start-ticket.
argument-hint: <issue number>
disable-model-invocation: true
---

# Start a ticket

Issue: $ARGUMENTS

The workflow and the naming rules are in `CONTRIBUTING.md` §2. This skill only orders the
steps.

1. **Read it**: `gh issue view <number> --comments`, then every document, ADR and spike it
   links, and the matching part of `docs/testing.md`.
2. **Stop and say so** if the ticket is not Ready (an open blocker, a *Done when* that cannot
   be checked), conflicts with an accepted ADR, is outside the V1 scope, or needs a
   **[to verify]** value to be true. Also stop if the working tree is not clean or another
   ticket is In Progress.
3. **Branch** from an up-to-date `main`, linked to the issue:

   ```bash
   git switch main && git pull --ff-only
   gh issue develop <number> --base main --name <type>/<number>-<short-topic> --checkout
   ```

   This creates the branch on GitHub as well as locally: running this skill is the
   maintainer's request for it.
4. **Board**: `gh issue edit <number> --add-assignee @me`, then
   `gh project item-edit <project number> --owner <owner> --url <issue url> --field Status --value "In Progress"`.
5. **Tell the maintainer**, in French: what the ticket asks, which documents apply, the *Done
   when* list, the first test to write, and anything unclear.
6. **Work test first** (`CONTRIBUTING.md` §4): write the failing test, show that it fails
   for the expected reason, then write the code. Keep both outputs for the pull request.

Do not commit or push the work unless asked. When it is finished, the maintainer runs
`/open-pr`.
