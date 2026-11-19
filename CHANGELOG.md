# Changelog

What changed in this project, newest first. One line for each change that someone using the
pin or the app would notice. A fix to a document, a refactoring or a test does not need one:
the pull request is its record.

A version is a release as defined in [docs/testing.md](docs/testing.md): a tagged state of
the repository that is flashed on the pin and installed on the phone. The protocol has its
own version number, noted here when it changes.

## Unreleased

### Added

- Design documentation: architecture, hardware, BLE protocol draft, companion app, firmware,
  display specification, power estimate, enclosure, safety, test strategy and roadmap.
- Architecture decision records 0001 to 0015 and spikes 0001 to 0012.
- Repository tooling: contribution rules, pull request and issue templates, rules for `main`,
  a CI workflow that checks the documentation, and `tools/check_docs.py`.
- Guide, skills and a documentation reviewer for AI agents.
