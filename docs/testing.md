# Testing

Status: **plan.** No code exists yet. This document says how each part of the project is
meant to be tested, so that the code is written to be testable from the first commit.

## Principles

1. **Hardware is the last thing a test should need.** The pin is not available yet, and
   later there will be exactly one. Everything that can be checked on a PC is checked on a PC.
2. **Logic lives outside the platform.** On the pin, the Arduino sketch is a thin shell
   around the display driver, GATT, buttons and sleep. On the phone, Android services are a
   thin shell around plain Kotlin. The logic in between is what gets tested.
3. **One contract, two implementations.** The protocol is implemented in Kotlin and in C++.
   Both are tested against the same files, so they cannot drift apart silently.
4. **Images are compared exactly.** The output is 1 bit per pixel and the algorithms are
   deterministic. A golden image either matches byte for byte or the test fails.
5. **What cannot be automated has a written checklist**, not a memory.
6. **Test first.** Logic is written with TDD: a failing test, the code that makes it pass,
   then the clean-up. The shared vectors and the golden images are the failing tests of the
   protocol and of the renderer.
7. **Whoever writes a reference does not approve it alone.** A golden image or a test vector
   produced by an agent is looked at by the maintainer before it is committed.

## Levels

| Level | What it proves | Runs on | When |
| --- | --- | --- | --- |
| 1. Unit tests | Each piece of logic does what it should | PC, CI | Every pull request |
| 2. Contract tests | Kotlin and C++ agree on every packet | PC, CI | Every pull request |
| 3. Golden images | The views render as expected | PC, CI | Every pull request |
| 4. Pipeline test | A bundle encoded by the app renders correctly in the simulator | PC, CI | Every pull request, once both sides exist |
| 5. BLE smoke test | The app talks GATT correctly to a real peripheral | Two devices, manual | Before merging a change to the BLE layer |
| 6. Hardware checklist | The real pin behaves as designed | Pin, manual | On reception, then before each release |
| 7. Power measurements | The consumption matches the budget | Pin and instrument, manual | On reception, then after any change flagged for power |

Levels 1 to 4 need no hardware and are the ones CI runs.

A **release** is a tagged state of the repository that is flashed on the pin and installed
on the phone for daily use. V1 is the first one.

## Level 1: unit tests

### Android (plain Kotlin, JVM)

These parts are written as pure functions so they run without a device or an emulator.

| Unit | What is tested |
| --- | --- |
| Session selection | Given a list of fake sessions: the playing one wins, Spotify has priority, an app that is not on the list of chosen apps is ignored, no session gives no track |
| Debounce and merge | With a virtual clock: a cover arriving within the wait is included, a second event restarts the wait, a fast series of skips sends only the last track |
| Deduplication | The same track reported twice, or a seek, sends nothing. A pause sends a state message and no bundle |
| Levels and dithering | A fixed 122 × 122 greyscale input gives the committed 1-bit output, byte for byte. Edge cases: all black, all white, a gradient |
| Bit packing | Rows padded to 16 bytes, 1,952 bytes in total, bit order as in the protocol |
| Share link | A track id gives the track link; no id gives a search link; title and artist are escaped; the scheme and host never change whatever the input; the result never exceeds the limit, including with multi-byte characters; a shortened link is still a valid link |
| Spotify broadcast | A malformed id is rejected; an id received while another app is shown is ignored; nothing displayed comes from the broadcast |
| Synchronisation | Given the pin's hello: the bundle is sent when the pin holds another one or none, and not sent when it already holds it; the playback state is sent in every case |

Two parts depend on Android and are kept thin:

- **Decoding and scaling the cover** uses Android bitmaps, whose output may differ between
  devices. It is not golden tested. The golden test starts after scaling, on a fixed
  greyscale array.
- **Text rendering** uses fonts bundled with the app, but Android's text engine still varies
  between versions, so a pixel-exact golden image would not be portable. The tests check
  properties instead, on the reference titles of [display-spec.md](display-spec.md): the
  bitmap is 128 × 122, the text stays inside its margins, the line counts follow the rules,
  an empty artist, a right-to-left title and an emoji all render without error. The visual
  check is done with the preview in the app.

### Firmware (portable C++, built on the PC)

| Unit | What is tested |
| --- | --- |
| Packet decoder | Covered by the contract tests below |
| Reassembly | Packets out of order, a duplicate, a missing chunk leading to a resend request, a packet whose counter differs from the bundle in progress dropped, a new bundle arriving in the middle of a transfer, CRC mismatch, an unknown protocol version refused |
| Share view | A link gives the QR code; no link gives no share view |
| QR code | The rendered image is decoded again by a QR reader and gives back the original link, for a short link and for the longest link allowed |
| Refresh triggers | With a clock passed as a parameter: one refresh per bundle, an icon drawn only after its delay, a burst of play and pause costs one refresh |
| Framebuffer | Pixel set and get, blit at an offset, clipping at the edges, conversion to the panel's memory layout |

The button handling and the state machine live in the portable code too, driven by a clock
passed as a parameter, so that long presses and delays are tested on the PC without waiting
for real time.

The host tests are built with the address and undefined-behaviour sanitizers. The packet
decoder and the reassembly code are also fuzzed: they are the only code on the pin that
reads bytes coming from the radio.

## Level 2: contract tests

The protocol test vectors are language-neutral files, kept in one place (proposed:
`tools/vectors/`). Each vector holds the decoded fields and the expected bytes.

Both implementations run every vector in both directions:

- fields → bytes must give exactly the expected bytes;
- bytes → fields must give exactly the expected fields.

Invalid vectors are part of the set, with the expected error: truncated packet, wrong CRC,
offset beyond the image, link longer than the limit, image size the pin cannot place,
unknown version.

Rule: a protocol change updates [protocol.md](protocol.md), the vectors and both
implementations in the same pull request. A vector that passes on one side only is a
failing build.

## Level 3: golden images

`firmware/sim` reads a test bundle and writes the framebuffer as an image.

- One golden image per view and per state: cover view, share view, paused, disconnected, low
  battery, both badges together, unpaired screen, pairing screen with a passkey, sleep image.
  The bundles come from the reference set of [display-spec.md](display-spec.md).
- Golden files are PBM: no codec, exact comparison.
- The test renders each bundle and compares it with the committed file, byte for byte.
- When a comparison fails, CI publishes the expected image, the rendered image and their
  difference as PNG files, because PBM files do not show in a pull request.
- Regenerating a golden image is a deliberate command. The new image is looked at before it
  is committed, and the pull request says why it changed.

## Level 4: pipeline test

This test connects the two halves without a radio:

1. A JVM test builds a bundle with the real encoder, from a cover bitmap and a text bitmap
   that are already 1 bit and committed as test data. It checks that the encoded bundle is
   identical to a committed file.
2. The simulator reads that same committed file through the real C++ decoder and reassembly
   code, and renders the view.
3. The result is compared with a golden image.

Neither side has to build or run the other: the committed file is the meeting point. Text
rendering and cover decoding are not part of this test, because they need Android.

## Test data

No third-party material is committed without a licence that allows it: no real cover art,
no logo, no file fetched from a music service. Covers used as test data are drawn for the
purpose, and titles are invented. Fonts are under an open licence and come with their
licence file. Logs of what was played on the phone are never committed.

## Level 5: BLE smoke test

Manual, a few minutes, against a real peripheral. Before the pin exists, the peripheral is a
second phone running nRF Connect as a GATT server.

- [ ] The app associates and asks for the bond.
- [ ] The MTU is negotiated and the app uses the value it was given.
- [ ] A track change produces the expected sequence of writes on the expected
      characteristics.
- [ ] A pause produces a state write and no bundle.
- [ ] The app reconnects after the peripheral goes out of range and comes back.

Limit: nRF Connect does not check the CRC, does not answer with an acknowledgement, does not
send the hello by itself and cannot require the passkey pairing. A phone acting as a
peripheral also changes its address, so bonding and reconnection are not reliable with it.
For this test the hello is sent by hand as a notification. Those paths are covered by levels
2 and 4, and this test moves to an nRF52840 board as soon as one is available.

### Endurance checklist

Before each release, with the real pin:

- [ ] The link survives the phone being locked for a night.
- [ ] The app comes back after its process is killed.
- [ ] The app comes back after Bluetooth is turned off and on.
- [ ] The app comes back after the phone restarts.
- [ ] The app says so when notification access is revoked.
- [ ] The app says so when the pin's bond was erased.
- [ ] The link survives the phone's deep idle mode.

## Level 6: hardware checklist

To run on reception, then before each release. Results are written in `docs/` with the date.

- [ ] Checks listed in [hardware.md](hardware.md).
- [ ] The cover view on the real panel matches the simulator image.
- [ ] Partial refresh on each track, full refresh after about 10 updates, no visible ghosting
      left after a full refresh.
- [ ] KEY1 short: view toggles without any radio activity.
- [ ] KEY1 long: sleep image, then System OFF, and no immediate wake.
- [ ] Every button wakes the pin from System OFF.
- [ ] After a wake, the sleep image stays until the phone sends the track.
- [ ] Out of range: the last track stays on screen and the icon appears after its delay.
      Back in range: reconnection and update.
- [ ] Security, from a second device:
  - [ ] paired without the passkey, it cannot read, write or subscribe, and its bond is
        deleted, while the real phone's bond is intact and the phone still reconnects;
  - [ ] the battery level cannot be read without the passkey pairing;
  - [ ] the device name cannot be written;
  - [ ] a pairing attempt outside the pairing window keeps no new bond and leaves the real
        phone's bond intact;
  - [ ] after the bond is erased, the former phone is refused;
  - [ ] the list of services holds no update service;
  - [ ] once bonded, the advertising shows no name and no service UUID;
  - [ ] a stranger that connects is dropped after about 5 s.
- [ ] Waking with each button starts the firmware, not an update mode.
- [ ] The QR code is read by a phone camera from the real panel.
- [ ] The battery level reported in the app follows the real voltage.
- [ ] Range with the pin on the chest and the phone in a back pocket, case closed, magnet
      clip in place.
- [ ] A full day of normal listening without a lost connection or a frozen screen.
- [ ] Checks listed in [safety.md](safety.md).

## Level 7: power

The measurement table is in [power.md](power.md). It is filled in on reception and measured
again after every change that was flagged as affecting consumption.

## Enclosure

- CI renders the parametric model from the command line: the script must build without error.
- Before a full print, small test pieces check the clearances: screen window, button caps or
  flexures, clips.
- A fit checklist is run on the printed case: the panel is supported and not clamped, the
  FPC curve is gentle, nothing presses on the battery, every opening lines up, pressing one
  button does not press the one opposite.

## Continuous integration

GitHub Actions, on every pull request to `main` and on `main` itself. One workflow,
`.github/workflows/ci.yml`. Its last job, `ci-ok`, succeeds only if all the others did, and
is the single check that the rules of `main` require.

| Job | Content | Exists |
| --- | --- | --- |
| lint | Every hook of `.pre-commit-config.yaml`, and the rendering of the Mermaid diagrams | yes |
| conventional-commit | The pull request title, which becomes the squashed commit, follows Conventional Commits | yes |
| android | Kotlin unit tests, debug build, Android lint, and a check that the built app does not declare the network permission | no |
| firmware-host | Build of the portable C++ on the PC with sanitizers, unit tests, contract vectors, golden images, pipeline test | no |
| firmware-sketch | Compilation of the Arduino sketch for the board | no |
| enclosure | Command-line render of the model | no |
| ci-ok | Succeeds only if every job above succeeded | yes |

A job is added in the same pull request as the first code it covers, and listed in the
`needs` of `ci-ok`. Every job always runs: `ci-ok` treats a skipped job as a failure. To
skip work when nothing relevant changed, the condition goes on the steps, never on the job
or on the workflow.

## Code quality tools

One formatter and one linter per language, no more. They are all hooks of
`.pre-commit-config.yaml`: the same file runs on every commit and in the `lint` job of CI. A
tool is added to it with the first code it checks, as part of the first task of the project:
the whole CI before anything else.

| Part | Tool | What it does | In place |
| --- | --- | --- | --- |
| Every file | pre-commit-hooks | Line endings, trailing spaces, large files, private keys, YAML and JSON syntax | yes |
| Markdown | markdownlint | Style of the documents | yes |
| Documents | `tools/check_docs.py` | Links, indexes, ruleset file | yes |
| Diagrams | `tools/check_mermaid.sh` | Every Mermaid diagram renders | yes |
| Commits | conventional-pre-commit | Each commit message is a conventional commit | yes |
| Workflows | actionlint, zizmor | Mistakes and security of the CI files themselves | yes |
| Python (`tools/`) | ruff | Formatting and lint, one tool for both | yes |
| Kotlin | ktlint | Formatting and style, one tool for both | no |
| Android | Android Lint | Platform mistakes, missing permissions | no |
| C++ | clang-format | Formatting | no |
| C++ | Compiler warnings as errors (`-Wall -Wextra -Werror`) and the sanitizers | Most real mistakes, with no extra tool | no |
| Project rules | A guard script that greps the code | What no linter knows: no Arduino include in the portable code, no network permission in the app, no update service in the firmware | no |

Left out on purpose until a real need shows: detekt and clang-tidy (slow, noisy on a small
code base), spell checking, and a coverage threshold.

## What is not tested automatically

| Item | Why | Covered by |
| --- | --- | --- |
| Real BLE link | Needs two devices | Levels 5 and 6 |
| E-ink rendering quality | Needs the panel and eyes | Level 6 |
| Power consumption | Needs an instrument | Level 7 |
| Text appearance | Android's text engine varies between versions | Preview in the app, and spike 0011 for legibility |
| Behaviour of other phone brands in the background | Needs those phones | Endurance checklist on the maintainer's phone only |

## Open decisions

To settle when the first code lands. They are recorded in the pull request that makes them;
an ADR is only written for a choice that would be costly to reverse.

- C++ test framework and PC build system for `firmware/`.
- File format of the protocol vectors, and whether golden images are plain or binary PBM.
- How the Android-dependent tests run (on the JVM with a simulated framework, or on an
  emulator).
- QR reader used by the round-trip test.
