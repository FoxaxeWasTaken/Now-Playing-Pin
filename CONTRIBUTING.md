# Contributing

This is a solo project. This file mostly records the working rules I hold myself to, so that
the repository stays consistent over time. It also explains what to expect if you want to
take part.

## If you are not me

- **Issues** are welcome: mistakes in the documentation, a wrong pinout, a question, an idea.
  I answer when I can.
- **Pull requests**: please open an issue first. I am not actively looking for outside
  contributions, and a pull request that arrives without discussion may stay unreviewed or be
  closed.
- The accepted decisions in [docs/adr/](docs/adr/README.md) are fixed. Proposals that go
  against them (iOS support, Wi-Fi on the pin, a progress bar, periodic refresh) will be
  declined.

## Workflow

- Work is tracked as GitHub issues on the "Now Playing Pin" project board.
- One branch per feature, created from `main` and named `<area>/<short-description>`, for
  example `firmware/qr-view` or `docs/protocol-framing`.
- A branch reaches `main` through a pull request. Nobody reviews it: the pull request exists
  so that CI runs before the merge, and to leave a record.
- The rules of `main` require the `ci-ok` check, a pull request and a squash merge. They are
  kept in [.github/rulesets/main.json](.github/rulesets/main.json).
- No direct commit on `main` after the initial one.

### Ready and done

A ticket is **ready** when the tickets it depends on are closed, its "Done when" list can be
checked, and it links the documents it relies on.

A ticket is **done** when its "Done when" list is checked, CI is green, the documents match
the change, and "Current state" in [CLAUDE.md](CLAUDE.md) is still true. A change that
someone using the pin or the app would notice also gets a line in
[CHANGELOG.md](CHANGELOG.md).

One ticket is one pull request. At most one ticket is in progress at a time.

### Pull request description

The description follows [the template](.github/pull_request_template.md): what and why, how
it was checked, what was left out. It is not written for a reviewer. It is the record that
will be read when the history is searched later. A documentation-only change needs two
lines.

### Labels and milestones

- One area: `area:docs`, `area:android`, `area:firmware`, `area:enclosure`, `area:hardware`,
  `area:tools`, `area:ci`.
- One type: `type:task`, `type:spike`, `type:bug`.
- `needs-hardware` when it cannot be finished without the pin. `power` when it affects the
  pin's consumption.
- Milestones are those of [docs/roadmap.md](docs/roadmap.md): `M1 Simulated pin`,
  `M2 Pin on the desk`, `M3 Wearable V1`, `Later`.

### Repository setup

Done once, with the GitHub CLI. Kept here so that it can be redone or checked. The CLI token
needs the `project` scope (`gh auth refresh -s project`).

```bash
# 1. After the first push of main, and once its CI run is green:
gh api repos/{owner}/{repo}/rulesets --method POST --input .github/rulesets/main.json
gh repo edit --enable-squash-merge --enable-merge-commit=false --enable-rebase-merge=false \
  --delete-branch-on-merge

# 2. Labels and milestones, one call for each name listed above:
gh label create "area:docs" --color 0E8A16
gh api repos/{owner}/{repo}/milestones --method POST -f title="M1 Simulated pin"

# 3. The board, linked to the repository:
gh project create --owner @me --title "Now Playing Pin"
gh project link <project number> --owner <owner> --repo <owner>/<repo>
```

The board has GitHub's three default columns: Todo, In Progress, Done. A "Waiting" column
and the fields `Size` and `Who` are added in the board's settings if they turn out to be
needed. The automatic addition of new issues is turned on in the same place.

To change the rules later, edit the JSON file and send it again:

```bash
gh api repos/{owner}/{repo}/rulesets                       # find the id
gh api repos/{owner}/{repo}/rulesets/<id> --method PUT --input .github/rulesets/main.json
```

Nobody can bypass the rules, the maintainer included. If CI itself is broken (an action that
no longer runs), the way out is to set `"enforcement": "disabled"` with the command above,
fix CI, and turn it back on.

When a CI job is added, nothing changes in the rules: `ci-ok` covers it.

## Commits

- [Conventional Commits](https://www.conventionalcommits.org/): `<type>(<scope>): <description>`,
  description in the imperative.
  - Types: `feat`, `fix`, `docs`, `test`, `refactor`, `perf`, `build`, `ci`, `chore`, `revert`.
  - Scope, optional, is the area touched: `android`, `firmware`, `protocol`, `enclosure`,
    `hardware`, `tools`.
  - A `!` before the colon marks a breaking change, such as a new protocol version.
- Pull requests are squashed, so the pull request title is the commit that lands on `main`.
  CI checks that title. The commits inside a branch are not checked.
- Never commit signing keys, `local.properties` or any credential.
- Never commit third-party material without a licence that allows it: no logo, no real cover
  art, no file fetched from a music service. A font goes in with its licence file.
- Never commit a log of what was played on the phone.

Example:

```text
fix(firmware): put QSPI flash in deep power-down after refresh
```

## Decisions and experiments

- A choice that is costly to reverse is recorded as an ADR in
  [docs/adr/](docs/adr/README.md). An accepted ADR is not edited: a new one supersedes it.
- A smaller choice is recorded in the pull request that makes it.
- A question that needs an experiment before a choice can be made is written as a spike in
  [docs/spikes/](docs/spikes/README.md), with a time box and an exit condition. A spike ends
  with a recorded result, even a negative one.
- A spike result is only written from an experiment that was actually run.

## Tests

The full strategy is in [docs/testing.md](docs/testing.md). The rules that apply to every
pull request:

- **Test first.** Logic is written with TDD: a failing test, the code that makes it pass,
  then the clean-up. This applies to everything that runs without hardware. It does not
  apply to documents, to throwaway spike code, to the enclosure model or to the thin layer
  that talks to the hardware.
- New logic comes with its tests in the same pull request.
- A protocol change updates [docs/protocol.md](docs/protocol.md), both implementations and
  the shared vectors together.
- A golden image is only regenerated on purpose, and the maintainer looks at the new image
  before it is committed.
- A bug found on the hardware that a host test could have caught gets a vector or a golden
  image before it is fixed.

Before pushing, run what CI runs:

```bash
python tools/check_docs.py
npx markdownlint-cli2 "**/*.md"   # needs Node.js; the editor's markdownlint extension reports the same
```

## Working rules

### Language

Everything in the repository is in English: code, identifiers, comments, commit messages and
documentation.

### Unverified values

- A value taken from a public page and not confirmed on a real part carries the tag
  **[to verify]**, and the document says where the value comes from. The tag is removed only
  once the value has been measured or checked on the part, and the pull request that removes
  it says how.
- Never invent a pinout, a vendor UUID or a dimension. An unknown value is written as unknown.
- Battery life numbers are labelled as estimates until they are measured. Measurements go in
  `docs/` with the date, the instrument and the method.

### Power

Battery life comes before features. Any change that increases the pin's consumption (shorter
connection interval, extra refresh, a peripheral left awake, a new radio feature) is called
out explicitly in the pull request and in [docs/power.md](docs/power.md) before it is merged.

### Code

- The rendering and protocol code is portable C++ with no Arduino dependency. It must build
  and run on a PC.
- The protocol encoder and decoder exist in Kotlin and in C++ and are tested against the
  same vectors.
- Nothing polls. The app and the firmware react to events.
- The enclosure is parametric: every dimension is a named variable at the top of the file.
- Nothing outside the V1 scope ([ADR-0010](docs/adr/0010-v1-scope.md)) is built before V1 is
  done.

## AI agents

Agents working in this repository follow [CLAUDE.md](CLAUDE.md), which summarises these
rules and points to the documents above.
