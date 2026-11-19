---
name: open-pr
description: Prepare and open a pull request for the current feature branch of the Now Playing Pin project, from the local checks to a green CI, and merge when asked. Run by the maintainer with /open-pr.
argument-hint: <issue number, optional>
disable-model-invocation: true
---

# Open a pull request

The rules are in `CONTRIBUTING.md` (§2 workflow, §2.1 naming, §4 tests, §6 documents, §7
Definition of Done) and the template is `.github/pull_request_template.md`. This skill only
orders the steps.

Pull requests here are not reviews. The description is a short, true record of the work.

## Before opening

1. **Read the whole diff**: `git diff main...HEAD` and `git status`. Describe what is there,
   not what was planned.
2. **Checks**, from the repository root: `pre-commit run --all-files`, then the tests of each
   part touched (`CLAUDE.md`, "Commands"). Keep the output.
3. **Documents the change makes wrong** (`CONTRIBUTING.md` §6): `git grep` each value, name,
   state and ticket number touched, in `README.md`, `CLAUDE.md` and `docs/`. Fix them in the
   present tense. `CHANGELOG.md` is not touched: it is written at a milestone.
4. **Stop and ask** if the diff contains something that was not agreed: more consumption for
   the pin, a **[to verify]** value used as a fact or a tag removed without a check on the
   part, a golden image or a test vector regenerated, third-party material, work outside V1
   or against an accepted ADR.

## Opening

1. **Title**: a conventional commit, `<type>(<scope>): <imperative sentence>`. It becomes the
   squash commit, and CI refuses any other form.
2. **Body** from the template:
   - one `Closes #<n>` line per ticket the branch finishes;
   - the evidence: the commands and their output, the RED then GREEN of the tests, the
     measurement. Only what was run in this session or shown by the maintainer;
   - what was not checked, and what is left for later;
   - the choices made where the ticket or a document was ambiguous.

   Never put a local path (`C:\…`, `/home/…`) in it: say how to reproduce instead. English,
   no praise, no account of the process.
3. Commit and push only if the maintainer asked for it, or asks now. Then:

   ```bash
   git push -u origin HEAD
   gh pr create --base main --title "<title>" --body-file <file> --assignee @me
   gh pr checks --watch
   ```

   The body goes in a temporary file outside the repository.
4. Move the ticket to **In review** on the board.

## After

- No checks listed right after the creation: wait a few seconds and try once more. Still
  none: CI did not start, look at `.github/workflows/ci.yml`.
- A check fails: read its log, explain the cause, fix it on the same branch.

  ```bash
  gh run list --branch <branch> --limit 1 --json databaseId,conclusion
  gh run view <run id> --log-failed
  ```

- **Merge** only when the maintainer asks, and only with `ci-ok` green:
  `gh pr merge --squash --delete-branch`, then `git switch main && git pull --ff-only`.
  Confirm the issue is closed and Done on the board.
