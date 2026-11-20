# Now Playing Pin: agent guide

Guide for AI agents working in this repository. It says where things are and which rules
apply. It does not repeat the design: read the linked document before working on an area.

## The project

A wearable e-ink pin (nRF52840 + 2.13" e-paper) that shows the music playing on an Android
phone: cover, title, artist. A button switches to a QR code for sharing the track. An
Android companion app does all the work and pushes ready-to-draw data over BLE.

## Current state

Keep this section up to date when it changes.

- Design phase. The repository contains documentation and build scaffolding only.
- No hardware has been ordered or measured.
- The builds exist, each with its CI job: the Android app (`android`, an empty screen, one
  test), the portable C++ (`firmware-host`, one test), the Arduino sketch
  (`firmware-sketch`, empty) and the enclosure (`enclosure`, the envelope only, OpenSCAD).
  No real code yet. The CI is complete: one job per part and every code quality tool of
  [docs/testing.md](docs/testing.md).
- Current milestone: M1 Simulated pin ([docs/roadmap.md](docs/roadmap.md)). The hardware
  pre-order checks run in parallel.
- [ADR-0012](docs/adr/0012-ble-security.md) (BLE security) is proposed, not accepted: it
  waits for spike 0012 on the hardware.
- The values in [docs/display-spec.md](docs/display-spec.md) are starting values, to confirm
  with spike 0011.
- Not decided: the list of apps shown on the pin.

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

- Work comes from GitHub issues. The issue body is the specification and its *Out of scope*
  section is binding.
- One branch per ticket, from `main`, named `<type>/<issue#>-<short-topic>`. The maintainer
  may put several related tickets on one branch, named after the first.
- Never commit on `main` directly. Changes reach `main` through a pull request, which runs CI
  and leaves a short record. Nobody reviews it.
- Commit and push only when the maintainer asks.
- Conventional Commits: `<type>(<scope>): <description>`, for example
  `feat(firmware): add the QR share view`. Types and scopes are listed in `CONTRIBUTING.md`.
  Pull requests are squashed, so the pull request title is the commit, and CI checks it.
  A local hook checks each commit message.
- A referenced document is ambiguous? Decide, say so in the pull request, and update that
  document in the same pull request. A conflict with an accepted ADR is different: stop.
- A pull request updates every document it makes wrong. Documents say what exists, in the
  present, never how it got there.
- Never commit signing keys, `local.properties` or credentials.
- Work is tracked as GitHub issues on the "Now Playing Pin" project board. A pull request
  closes one ticket, or several related ones, and one branch is in progress at a time.

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
- Work test first (TDD): write the failing test, show that it fails, then write the code.
  Not for documents, throwaway spike code, the enclosure model or the hardware layer.
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
| A milestone is reached | `CHANGELOG.md`, from the titles of the merged pull requests |

## Commands

```bash
pre-commit run --all-files     # every lint and check, exactly as in CI
bash tools/check_mermaid.sh    # every Mermaid diagram renders (needs Docker)

# Portable C++ (firmware/render, firmware/sim, firmware/tests), on Linux or in WSL:
cmake -S firmware -B build/firmware                   # configure: warnings as errors, sanitizers
cmake --build build/firmware --parallel               # build
ctest --test-dir build/firmware --output-on-failure   # run the tests
./build/firmware/sim                                  # run the simulator

# Android app, from android/, on Linux or in WSL (JDK 21, Android SDK, see Known traps):
./gradlew testDebugUnitTest      # JVM unit tests
./gradlew assembleDebug          # build app/build/outputs/apk/debug/app-debug.apk
./gradlew lintDebug              # Android Lint, fails on an error

# Arduino sketch, from the repository root (arduino-cli 1.5.1, see Known traps):
arduino-cli compile --fqbn Seeeduino:nrf52:xiaonRF52840Plus --library firmware/render firmware/pin

# Enclosure, from the repository root (OpenSCAD 2021.01):
mkdir -p build/enclosure && openscad --hardwarnings -o build/enclosure/case.stl enclosure/case.scad
```

When a part gets its build, record here the exact commands to build it and to run its tests.

## Known traps

What has already cost time, or would. Add to this list when a new one is found.

- A Markdown file can pass the editor's linter and still fail in CI: run pre-commit.
- `ci-ok` treats a skipped job as a failure. A condition goes on the steps of a job, never on
  the job itself.
- Fixing a pull request title reruns CI by itself. Nothing has to be pushed.
- `main` cannot be pushed to, by anyone. Every change goes through a pull request.
- The host build of the C++ needs GCC or Clang: the sanitizer flags do not exist in MSVC.
  On Windows, build and test in WSL.
- `pre-commit run --all-files` only sees files known to git: `git add` a new file before
  running it, or its hooks are skipped.
- The Kotlin hook (ktlint) runs Java: committing a `.kt` file needs Java on the PATH of the
  machine that commits.
- The Android build finds the SDK through `ANDROID_HOME` or `android/local.properties`
  (`sdk.dir=...`), which stays local. `sdkmanager --licenses` no longer does anything: the
  platform is installed with the Android CLI of the command-line tools, with
  `android --no-metrics sdk install "platforms;android-37.0"`, or Gradle cannot build.
- The Arduino core needs, on Linux, `adafruit-nrfutil` (0.5.3.post16, with pipx) and a
  `python` command, even just to compile. Install the core with the board index URL of
  `.github/workflows/ci.yml` and the version it pins.
- Not tried on hardware yet, read in sources ([docs/adr/0012-ble-security.md](docs/adr/0012-ble-security.md),
  [docs/firmware.md](docs/firmware.md)): the Bluetooth library accepts every pairing request;
  a wake from System OFF is a restart with a button held; the XIAO's flash draws current
  until it is put to sleep; the MTU is negotiated, never 247 by default.

## Area notes

**Android (`android/`)**: Kotlin, installed by hand. The Spotify broadcast receiver is
registered in code and its content is not trusted. Image and text processing happen here,
not on the pin, following `docs/display-spec.md`. Only chosen apps are shown. No network
permission in V1.

**Firmware (`firmware/`)**: Arduino, Seeed nRF52 core with Bluefruit. The screen is used in
landscape, 250 × 122. `firmware/render/` has no Arduino dependency and must build on a PC.
After each refresh, the display and the QSPI flash go to deep sleep.

**Enclosure (`enclosure/`)**: OpenSCAD 2021.01
([ADR-0016](docs/adr/0016-openscad-for-the-enclosure.md)), every dimension a named variable at
the top of the file.
