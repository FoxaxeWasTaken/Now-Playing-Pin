---
name: open-pr
description: Open a pull request for the current feature branch of the Now Playing Pin project, with the template filled from the real diff and the real test results, then follow CI and merge when asked. Run by the maintainer with /open-pr.
argument-hint: <issue number, optional>
disable-model-invocation: true
---

# Open a pull request

Pull requests here are not reviews. One person writes and merges. The pull request is a
short written record of the work and the gate where CI runs. Keep the description short and
true.

## Checks before opening

1. The branch is not `main` and is named `<area>/<short-description>`.
2. Read the whole diff: `git diff main...HEAD` and `git status`. Describe what is there, not
   what was planned.
3. Run the checks listed under "Commands" in `CLAUDE.md` and keep their output.
4. Go through the "Keep in sync" table of `CLAUDE.md` and fix what is missing. If someone
   using the pin or the app would notice the change, add one line under "Unreleased" in
   `CHANGELOG.md`, in the section that fits (Added, Changed, Fixed, Removed). Otherwise
   leave the changelog alone.
5. Look for these in the diff, and stop to ask if one is found and was not agreed:
   - anything that can increase the pin's consumption;
   - a **[to verify]** value used as a fact, or a tag removed without a check on the part;
   - a golden image or a test vector regenerated;
   - third-party material (a logo, real cover art, a file fetched from a music service);
   - work outside the V1 scope, or against an accepted ADR.

## Write the description

Fill `.github/pull_request_template.md`.

- "What and why": two to five lines, and `Closes #<number>`.
- "Verification": only what was run in this session or shown by the maintainer, with the
  result. Say what was not checked. For a documentation-only change, "docs only".
- "Left for later": the shortcuts and gaps, or "Nothing". Propose a ticket for each one that
  matters.
- "Flags": tick what applies, with one line of detail for each ticked box. A choice worth
  remembering goes here with its reason, unless it has its own ADR.
- English. No praise, no filler, no account of the process.

Title: the squash commit subject, as a conventional commit:
`<type>(<scope>): <imperative sentence>`, with the types and scopes of `CONTRIBUTING.md`.
CI refuses any other form.

## Open

Commit and push only if the maintainer asked for it, or asks now.

```bash
git push -u origin HEAD
gh pr create --base main --title "<title>" --body-file <file> --assignee @me
gh pr checks --watch
```

Write the body to a temporary file outside the repository. Report the URL and the CI result.

If a check fails, find the run and read its log, explain the cause, and fix it on the same
branch:

```bash
gh run list --branch <branch> --limit 1 --json databaseId,conclusion
gh run view <run id> --log-failed
```

If `gh pr checks` reports that there are no checks, they may not be registered yet: wait a
few seconds and try once more. If there are still none, CI did not start: say so and look at
`.github/workflows/ci.yml` instead of waiting.

## Merge

Only when the maintainer asks, and only with the `ci-ok` check green:

```bash
gh pr merge --squash --delete-branch
git switch main && git pull --ff-only
```

Then confirm the issue is closed and shown as done on the project board.
