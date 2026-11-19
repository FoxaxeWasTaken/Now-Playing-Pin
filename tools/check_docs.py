#!/usr/bin/env python3
"""Checks that the documentation is consistent with itself.

- every relative Markdown link points to an existing file;
- every ADR and spike file is listed in its index, with the same status;
- the ruleset file is valid JSON.

Also prints the number of "[to verify]" tags per file, for information.

Standard library only. Run from the repository root: python tools/check_docs.py
"""

from __future__ import annotations

import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LINK = re.compile(r"\[[^\]]*\]\(([^)\s]+)\)")
FENCE = re.compile(r"^(```|~~~)")
STATUS = re.compile(r"^- Status:\s*(.+?)\s*$", re.MULTILINE)
NUMBERED = re.compile(r"^(\d{4})-.+\.md$")


def markdown_files() -> list[Path]:
    """Tracked and untracked Markdown files, ignoring what git ignores."""
    out = subprocess.run(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard", "*.md"],
        cwd=ROOT, capture_output=True, text=True, check=True,
    ).stdout
    return [ROOT / line for line in out.splitlines() if (ROOT / line).is_file()]


def prose(text: str) -> str:
    """The text without fenced code blocks, where links are examples."""
    kept, fenced = [], False
    for line in text.splitlines():
        if FENCE.match(line.strip()):
            fenced = not fenced
            continue
        if not fenced:
            kept.append(line)
    return "\n".join(kept)


def check_links(files: list[Path]) -> list[str]:
    errors = []
    for path in files:
        for target in LINK.findall(prose(path.read_text(encoding="utf-8"))):
            if re.match(r"^[a-z][a-z0-9+.-]*:", target) or target.startswith("#"):
                continue
            file_part = target.split("#", 1)[0]
            if not exists_exact(path.parent, file_part):
                errors.append(f"{path.relative_to(ROOT)}: broken link to {target}")
    return errors


def exists_exact(base: Path, relative: str) -> bool:
    """True if the path exists with exactly this spelling.

    Windows ignores case, the CI runner does not: a link with the wrong case must fail here
    too.
    """
    current = base
    for part in Path(relative).parts:
        if part in (".", ""):
            continue
        if part == "..":
            current = current.parent
            continue
        if not current.is_dir() or part not in {entry.name for entry in current.iterdir()}:
            return False
        current = current / part
    return True


def check_index(directory: str, label: str) -> list[str]:
    """Each numbered file must appear in README.md with its own status."""
    errors = []
    folder = ROOT / directory
    index = (folder / "README.md").read_text(encoding="utf-8")
    numbers: dict[str, str] = {}
    for path in sorted(folder.glob("*.md")):
        match = NUMBERED.match(path.name)
        if not match:
            continue
        number = match.group(1)
        if number in numbers:
            errors.append(f"{directory}: number {number} used by {numbers[number]} and {path.name}")
        numbers[number] = path.name
        status = STATUS.search(path.read_text(encoding="utf-8"))
        if not status:
            errors.append(f"{directory}/{path.name}: no '- Status:' line")
            continue
        row = next(
            (line for line in index.splitlines()
             if line.startswith("|") and f"]({path.name})" in line),
            None,
        )
        if row is None:
            errors.append(f"{directory}/README.md: {label} {path.name} is not in the index")
        elif status.group(1) not in row:
            errors.append(
                f"{directory}/README.md: {path.name} is '{status.group(1)}' in its file, "
                "not in the index"
            )
    return errors


def check_ruleset() -> list[str]:
    path = ROOT / ".github" / "rulesets" / "main.json"
    try:
        json.loads(path.read_text(encoding="utf-8"))
    except (OSError, ValueError) as error:
        return [f"{path.relative_to(ROOT)}: {error}"]
    return []


def main() -> int:
    files = markdown_files()
    errors = (
        check_links(files)
        + check_index("docs/adr", "ADR")
        + check_index("docs/spikes", "spike")
        + check_ruleset()
    )

    tags = {
        str(p.relative_to(ROOT)).replace("\\", "/"): p.read_text(encoding="utf-8").count("[to verify")
        for p in files
    }
    total = sum(tags.values())
    print(f"{len(files)} Markdown files checked, {total} '[to verify]' tags:")
    for name, count in sorted(tags.items()):
        if count:
            print(f"  {count:3d}  {name}")

    for error in errors:
        print(f"ERROR {error}", file=sys.stderr)
    if errors:
        print(f"{len(errors)} error(s)", file=sys.stderr)
        return 1
    print("OK")
    return 0


if __name__ == "__main__":
    sys.exit(main())
