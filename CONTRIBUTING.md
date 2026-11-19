# Contributing

This is a solo project. This file records the working rules I hold myself to, so that the
repository stays consistent over time. It also explains what to expect if you want to take
part.

## If you are not me

- **Issues** are welcome: mistakes in the documentation, a wrong pinout, a question, an idea.
  I answer when I can.
- **Pull requests**: please open an issue first. I am not actively looking for outside
  contributions, and a pull request that arrives without discussion may stay unreviewed or be
  closed.
- The accepted decisions in [docs/adr/](docs/adr/README.md) are fixed. Proposals that go
  against them (iOS support, Wi-Fi on the pin, a progress bar, periodic refresh) will be
  declined.

## 1. Setup (once per clone)

```bash
pipx install pre-commit
pre-commit install --install-hooks -t pre-commit -t commit-msg
```

The hooks run the same checks as CI on every commit, and refuse a commit message that is not
a conventional commit.

## 2. Workflow

1. On the [project board](https://github.com/users/FoxaxeWasTaken/projects/8), take a ticket
   from **Ready**, highest priority first, and move it to **In Progress** (§7). One branch in
   progress at a time.
2. Create a branch named after the issue: `git switch -c feat/<issue#>-<topic>` (§2.1).
3. **Write the test first** (§4), then the code.
4. Run the checks: `pre-commit run --all-files`, and the tests of the part you touched.
5. Commit with a **conventional message** (§2.1). The hook rejects anything else.
6. Open the pull request with one `Closes #<issue#>` line per ticket and the **evidence**: the
   commands run and their output, or the measurement. The ticket moves to **In review**.
7. When CI is green, **squash merge**. The ticket closes and moves to **Done**, the branch
   is deleted.

Nobody reviews a pull request here. It exists so that CI runs before the merge, and to leave
a short record of what was done, how it was checked and what was left out
([template](.github/pull_request_template.md)). A documentation-only change needs two lines.

### 2.1 Naming: branches, commits, pull request titles

| What | Format | Example | Checked by |
| --- | --- | --- | --- |
| Branch | `<type>/<issue#>-<short-topic>`, lowercase with hyphens. The number is the first ticket of the branch | `feat/12-qr-share-view` | nobody |
| Commit | `<type>(<scope>): <description>` | `fix(firmware): keep the bond when a stranger pairs` | `commit-msg` hook, locally |
| Pull request title | Same format: it becomes the squash commit on `main` | `feat(android): dither the cover` | CI, job `conventional-commit` |

- **Types**: `feat` new behaviour · `fix` bug fix · `docs` · `test` · `refactor` no change of
  behaviour · `perf` · `build` dependencies, packaging · `ci` workflows, hooks · `chore`
  housekeeping · `revert`.
- **Scope**, optional and lowercase, is the area touched: `android`, `firmware`, `protocol`,
  `enclosure`, `hardware`, `tools`.
- **Description**: imperative ("add", not "added"), lowercase start, no final period.
- **Breaking change**, such as a new protocol version: `!` before the colon, explained in the
  pull request.
- [Conventional Commits](https://www.conventionalcommits.org/) is the reference.

### 2.2 The rules of `main`

Kept in [.github/rulesets/main.json](.github/rulesets/main.json): a pull request, the `ci-ok`
check, squash merge only, no force-push, no deletion. Nobody can bypass them, the maintainer
included. If CI itself is broken (an action that no longer runs), the way out is to set
`"enforcement": "disabled"` with the command of §8, fix CI, and turn it back on.

## 3. Rules that do not bend

- **Stay inside V1** ([ADR-0010](docs/adr/0010-v1-scope.md)). Nothing outside it is built
  before V1 is done.
- **Never present an unverified value as a fact.** A value read on a public page carries the
  tag **[to verify]** and the document says where it comes from. The tag goes only when the
  value was checked on the real part, and the pull request says how. Never invent a pinout,
  a UUID or a dimension.
- **Battery life comes before features.** Any change that increases the pin's consumption
  (shorter connection interval, extra refresh, a peripheral left awake, a new radio feature)
  is called out in the pull request and in [docs/power.md](docs/power.md) before it is merged.
- **No third-party material without a licence that allows it**: no logo, no real cover art,
  no file fetched from a music service. A font goes in with its licence file.
- **Never commit** signing keys, `local.properties`, a credential, or a log of what was
  played on the phone.
- **Security is built as proposed** in [ADR-0012](docs/adr/0012-ble-security.md) until its
  spike confirms or replaces it.

## 4. Tests

The full strategy is in [docs/testing.md](docs/testing.md).

**Test first (TDD).** For a new behaviour or a bug fix:

1. write the test that shows it;
2. run it and watch it fail, for the expected reason (not a build error or a typo);
3. write the code until it passes, then run the whole suite of that part.

The pull request shows both runs, RED then GREEN. A bug gets its regression test before its
fix. A test that passes at once proves nothing: change it until it fails without the code.

This applies to everything that runs without hardware. It does not apply to documents, to
throwaway spike code, to the enclosure model or to the thin layer that talks to the hardware.

- A protocol change updates [docs/protocol.md](docs/protocol.md), both implementations and
  the shared vectors together.
- A golden image is only regenerated on purpose, and the maintainer looks at the new image
  before it is committed.
- A bug found on the hardware that a host test could have caught gets a vector or a golden
  image before it is fixed.
- A flaky test never stays in `main`: fix the cause or the test, never rerun until it passes.

## 5. Style and quality

- One formatter and one linter per language, all run by pre-commit, locally and in CI
  ([docs/testing.md](docs/testing.md)). A tool is added to `.pre-commit-config.yaml` with the
  first code it checks.
- Logic lives outside the platform: plain Kotlin outside Android services, portable C++
  outside the Arduino sketch. The rendering and protocol code must build and run on a PC.
- Nothing polls. The app and the firmware react to events.
- The enclosure is parametric: every dimension is a named variable at the top of the file.
- Everything in the repository is in English: code, identifiers, comments, commit messages
  and documentation.

## 6. Decisions and documentation

- **A pull request updates every document it makes wrong, in the same pull request**, not
  only the one next to the file it changes. Before opening it, `git grep` what the change
  touches (a value, a name, a state, a ticket number) in `README.md`, `CLAUDE.md` and `docs/`.
- **Documents say what exists, in the present**, never the path that led there ("the link is
  78 bytes at most", not "the limit was lowered from 100"). Git and the changelog keep the
  history. Dated records stay dated: ADRs, spike results, measurements.
- A choice that is costly to reverse gets an **ADR** ([docs/adr/](docs/adr/README.md)). An
  accepted ADR is not edited: a new one supersedes it. A smaller choice is recorded in the
  pull request that makes it.
- A question that needs an experiment gets a **spike** ([docs/spikes/](docs/spikes/README.md)),
  with a time box and an exit condition. Its result is only written from an experiment that
  was actually run, even when the answer is negative.
- Measurements go in `docs/` with the date, the instrument and the method. Battery life
  numbers are labelled as estimates until they are measured.
- Diagrams are **Mermaid**, and CI checks that they render.
- [CHANGELOG.md](CHANGELOG.md) is written when a milestone is reached, from the titles of
  the merged pull requests. It is not touched in between.

## 7. Tickets and the project board

All planned work lives on one GitHub Project linked to this repository.

- **A ticket is half a day to three days of work.** Bigger? Split it. Smaller? Fold it into
  the ticket next to it, unless it is a bug.
- **A pull request closes one ticket**, or several related ones when a CI run for each would
  cost more than it gives. The branch then carries the number of the first.
- **Statuses**: Backlog → **Ready** → In Progress → In review → Done.
- **Milestone**: the outcome the ticket serves, from [docs/roadmap.md](docs/roadmap.md):
  `M1 Simulated pin`, `M2 Pin on the desk`, `M3 Wearable V1`, `Later`. Every ticket has one.
- **Fields**: Priority (P0 critical path · P1 needed for V1 · P2 optional), Size (XS under
  2 h · S half a day · M one day · L two to three days), Who (Me · Agent · Hands-on).
  Dependencies use GitHub's native **blocked by** relationship, not labels.
- **Labels**: one type (`task`, `spike`, `bug`), one or more areas (`area:android`,
  `area:firmware`, `area:protocol`, `area:enclosure`, `area:hardware`, `area:tools`,
  `area:ci`, `area:docs`), `needs-hardware` when it cannot be finished without the pin, and
  `power` when it affects the pin's consumption.

**Anatomy of a ticket** ([templates](.github/ISSUE_TEMPLATE/)): Goal · Context · Depends on ·
Done when · Out of scope · Files and interfaces. Write it so that **an AI agent can do it
without asking**: exact paths, checkable criteria (a command, a test, a threshold), what not
to touch.

**Definition of Ready** (Backlog → Ready): it has a milestone; every *blocked by* ticket is
Done; *Done when* is checkable; the files and documents it touches are named.

**Definition of Done**, on top of the ticket's own *Done when*:

- [ ] Merged through a pull request with green CI.
- [ ] The rules of §3 respected.
- [ ] A test covers the new behaviour, and the pull request shows RED then GREEN.
- [ ] Every document the change made wrong is updated, and "Current state" in
      [CLAUDE.md](CLAUDE.md) is still true.

**Working with an AI agent**: give it the issue. It reads [CLAUDE.md](CLAUDE.md) and this
file. The ticket body is the specification and *Out of scope* is binding. If a referenced
document is ambiguous, the agent decides, says so in the pull request and updates that
document in the same pull request.

## 8. Repository setup

Done once, with the GitHub CLI. Kept here so that it can be redone or checked. The CLI token
needs the `project` scope (`gh auth refresh -s project`).

```bash
# Merge settings, then the rules of main once its first CI run is green:
gh repo edit --enable-squash-merge --enable-merge-commit=false --enable-rebase-merge=false \
  --delete-branch-on-merge
gh api repos/{owner}/{repo}/rulesets --method POST --input .github/rulesets/main.json

# To change the rules later, edit the JSON file and send it again:
gh api repos/{owner}/{repo}/rulesets                       # find the id
gh api repos/{owner}/{repo}/rulesets/<id> --method PUT --input .github/rulesets/main.json

# Labels and milestones, one call for each name of §7:
gh label create "area:firmware" --color 0E8A16
gh api repos/{owner}/{repo}/milestones --method POST -f title="M1 Simulated pin"
```

The board is a copy of another project of the same owner, which brings its views, its
fields and its automations (`gh project copy`). Automations cannot be turned on through the
API: "Auto-add to project" is switched on by hand in the board's settings, under Workflows.

When a CI job is added, nothing changes in the rules: `ci-ok` covers it.

## AI agents

Agents working in this repository follow [CLAUDE.md](CLAUDE.md), which summarises these
rules and points to the documents above. The recurring procedures (a ticket, a pull request,
an ADR, a spike) also exist as skills in `.claude/skills/`: each gives the order of the steps
and points to this file, which stays the reference.
