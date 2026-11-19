# Now Playing Pin: agent guide

Guide for AI agents working in this repository. It says where things are and which rules
apply. It does not repeat the design: read the linked document before working on an area.

## The project

A wearable e-ink pin (nRF52840 + 2.13" e-paper) that shows the music playing on an Android
phone: cover, title, artist. A button switches to a QR code for sharing the track. An
Android companion app does all the work and pushes ready-to-draw data over BLE.

## Current state

Keep this section up to date when it changes.

- Design phase. The repository contains documentation and repository tooling only.
- No hardware has been ordered or measured.
- No application or firmware code exists. CI only checks the documentation.
- Current milestone: M1 Simulated pin ([docs/roadmap.md](docs/roadmap.md)).
- **The first task is to set up the whole CI**: scaffolding, code quality tools and one job
  per part ([docs/testing.md](docs/testing.md)). Remind the maintainer of this if another
  task is about to start before it is done. The hardware pre-order checks run in parallel.
- [ADR-0012](docs/adr/0012-ble-security.md) (BLE security) is proposed, not accepted: it
  waits for spike 0012 on the hardware.
- The values in [docs/display-spec.md](docs/display-spec.md) are starting values, to confirm
  with spike 0011.
- Not decided: the minimum Android version, the list of apps shown on the pin.

## Where things are

| Need | Read |
| --- | --- |
| What V1 is, milestones, order of work | [docs/roadmap.md](docs/roadmap.md) |
| Overview, who does what | [docs/architecture.md](docs/architecture.md) |
| Parts, what is known and from where | [docs/hardware.md](docs/hardware.md) |
| BLE security, connection sequence, messages | [docs/protocol.md](docs/protocol.md) |
| Companion app | [docs/android-app.md](docs/android-app.md) |
| Views, buttons, states, refresh policy | [docs/firmware.md](docs/firmware.md) |
| What is drawn where, text and image rules | [docs/display-spec.md](docs/display-spec.md) |
| Power estimate and measurements | [docs/power.md](docs/power.md) |
| Case requirements | [docs/enclosure.md](docs/enclosure.md) |
| Battery, magnets, wearing the pin | [docs/safety.md](docs/safety.md) |
| How everything is tested | [docs/testing.md](docs/testing.md) |
| Why a choice was made | [docs/adr/](docs/adr/README.md) |
| Questions waiting for an experiment | [docs/spikes/](docs/spikes/README.md) |
| Branches, commits, labels, working rules | [CONTRIBUTING.md](CONTRIBUTING.md) |
| Pull request and issue templates | [.github/](.github/) |
| Rules applied to `main` on GitHub | [.github/rulesets/main.json](.github/rulesets/main.json) |
| Project skills and agents | [.claude/](.claude/) |

Planned directories, created with their first real file: `android/`, `firmware/render/`,
`firmware/sim/`, `enclosure/`.

## Rules

1. **Accepted decisions stay fixed.** They are in [docs/adr/](docs/adr/README.md). Do not
   work around one. If a task conflicts with one, stop and say so.
2. **Stay inside V1.** [ADR-0010](docs/adr/0010-v1-scope.md) lists what V1 contains. Do not
   build the Spotify Code, media control from the pin, settings or NFC before V1 is done.
3. **Never present an unverified value as a fact.** Values tagged **[to verify]** keep their
   tag until they are checked on the real part. A value read on a web page is recorded with
   its source and keeps its tag. Never invent a pinout, a UUID, a dimension or a
   measurement. If a value is unknown, write that it is unknown.
4. **Estimates are labelled as estimates**, battery life first of all.
5. **Flag power cost before adding it.** Anything that increases the pin's consumption is
   raised with the maintainer before it is written, then recorded in `docs/power.md`.
6. **Do not fake evidence.** A spike's Result section and the measurement tables are filled
   only from experiments that were run. Leave them as "Not run" or "not measured" otherwise.
7. **No third-party material without a licence that allows it.** No logo, no real cover art,
   no file fetched from a music service is committed. Fonts are under an open licence and
   come with their licence file. Logs of what was played on the phone are never committed.
8. **Do not weaken security on your own.** The rules of
   [ADR-0012](docs/adr/0012-ble-security.md) are still a proposal, but they are the ones to
   build to until spike 0012 confirms or replaces them: nothing on the pin readable or
   writable without the passkey pairing, and no update service started.

## Language

- Reply to the maintainer in French.
- Everything written into the repository is in English: code, identifiers, comments,
  documentation, commit messages, pull request text.

## Workflow

- One branch per feature, from `main`, named `<area>/<short-description>`.
- Never commit on `main` directly. Changes reach `main` through a pull request, which runs CI
  and leaves a short record. Nobody reviews it.
- Commit and push only when the maintainer asks.
- Commit subject in the imperative with an area prefix: `docs:`, `android:`, `firmware:`,
  `enclosure:`, `hardware:`, `tools:`, `ci:`.
- Never commit signing keys, `local.properties` or credentials.
- Work is tracked as GitHub issues on the "Now Playing Pin" project board. One ticket is one
  pull request, and one ticket is in progress at a time.

## Who does what

- The maintainer decides: scope, ADRs, which ticket is next, when to merge.
- The maintainer does what needs hands or eyes: measurements, anything on the hardware,
  judging an image on the panel or in the simulator.
- An agent can write code, tests and documentation for a ticket that is ready.
- An agent does not approve its own references. A golden image, a test vector or a spike
  result produced in a session is shown to the maintainer before it is committed.

## Skills and agents

Use the project skill when one fits, instead of improvising the steps.

| Skill | Use it to | Started by |
| --- | --- | --- |
| `new-ticket` | Create an issue with the right template, labels and milestone | The maintainer, `/new-ticket` |
| `start-ticket` | Read a ticket, create its branch, mark it in progress | The maintainer, `/start-ticket` |
| `open-pr` | Open the pull request, follow CI, merge when asked | The maintainer, `/open-pr` |
| `new-adr` | Record or supersede a decision that is costly to reverse | Either |
| `spike` | Plan a spike, or write up the result of one that was run | Either |
| `confirm-value` | Replace a **[to verify]** value checked on the real part, everywhere | Either |
| `project-status` | Say where the project stands and what is next | Either |

The `docs-reviewer` agent re-reads documentation changes without editing them. It runs when
the maintainer asks for it, typically after a large change to the design documents.

## Decisions

- A choice that is costly to reverse is an ADR. A smaller one is recorded in the pull
  request that makes it.
- If the choice needs an experiment first, write a spike and do not guess its outcome.
- An accepted ADR is never edited to change its decision. Write a new one that supersedes it.

## Tests

Read [docs/testing.md](docs/testing.md) before writing code. In short:

- Write logic so it runs without hardware: plain Kotlin outside Android services, portable
  C++ outside the Arduino sketch.
- New logic comes with its tests in the same change.
- The protocol exists in Kotlin and in C++ and both are tested against the same vectors.
- Golden images are compared byte for byte and are never regenerated just to make a test pass.

## Keep in sync

| When this changes | Also update |
| --- | --- |
| The protocol | `docs/protocol.md`, both implementations, the shared vectors |
| A document is added or renamed | The documentation table in `README.md` and the table above |
| An ADR or a spike is added or changes status | Its index (checked by `tools/check_docs.py`) |
| A **[to verify]** value is checked on the part | Every document that carries it, and say how it was checked |
| The V1 scope | `docs/adr/` (a new ADR), `docs/roadmap.md`, and the "Later" sections of the design documents |
| A label, a milestone or the name of the board | `CONTRIBUTING.md`, which the skills refer to |
| A CI job is added | The `needs` of `ci-ok` in `.github/workflows/ci.yml`, the CI table in `docs/testing.md`, "Commands" below |
| The project moves to another milestone | "Current state" above and the status in `README.md` |
| A change someone using the pin or the app would notice | One line under "Unreleased" in `CHANGELOG.md`, in the same pull request |

## Commands

```bash
python tools/check_docs.py          # relative links, ADR and spike indexes, ruleset file
npx markdownlint-cli2 "**/*.md"     # Markdown lint, as in CI (needs Node.js)
```

When a build system lands, record here the exact commands to build and to run the tests of
each part.

## Area notes

**Android (`android/`)**: Kotlin, installed by hand. The Spotify broadcast receiver is
registered in code and its content is not trusted. Image and text processing happen here,
not on the pin, following `docs/display-spec.md`. Only chosen apps are shown. No network
permission in V1.

**Firmware (`firmware/`)**: Arduino, Seeed nRF52 core with Bluefruit. The screen is used in
landscape, 250 × 122. `firmware/render/` has no Arduino dependency and must build on a PC.
After each refresh, the display and the QSPI flash go to deep sleep.

**Enclosure (`enclosure/`)**: parametric CAD as code, every dimension a named variable at the
top of the file. The tool is not chosen yet.
