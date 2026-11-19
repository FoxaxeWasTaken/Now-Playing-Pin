---
name: spike
description: Plan a spike (a time-boxed experiment answering one question) for the Now Playing Pin project, or write up the result of a numbered spike that was run. Use when a choice cannot be made without trying something, or when the maintainer asks to plan a spike or to record the result of spike NNNN.
argument-hint: new <question> | result <spike number>
---

# Spikes

Request: $ARGUMENTS

Read `docs/spikes/README.md` first (rules, template, index).

## Plan a new spike

1. Check that no existing spike covers the question.
2. Write `docs/spikes/NNNN-short-title.md` from the template:
   - one question, answerable by an experiment;
   - why the answer changes a decision, and which document it feeds;
   - a method made of steps someone can follow;
   - "Done when": the observable condition that ends it;
   - whether it needs the pin;
   - a time box, marked as a suggestion unless the maintainer gave it.
3. Leave "Result" as `Not run.` and status as `Planned`.
4. Add it to the index, and to `docs/roadmap.md` under the milestone it belongs to.
5. Run `python tools/check_docs.py`.

## Record a result

1. The result comes from an experiment that was actually run: by the maintainer, or in this
   session with the output visible. If there is no such evidence, do not write a result. Ask
   for the data. Something read or reported without running the experiment goes under
   "Notes".
2. Fill "Result" with: the date, the conditions (phone model, Android version, firmware
   commit, instrument), the raw observations, then the conclusion. Keep the numbers as
   measured. A negative or inconclusive answer is a valid result and is written as such.
3. Fill "Outcome": the documents updated, and the new ADR if the result leads to a decision
   or changes one. An accepted ADR is never edited.
4. Set the status to `Done` or `Abandoned`, in the file and in the index.
5. If the result gives a value that the documents carry as **[to verify]** or "not
   measured", update it everywhere with the `confirm-value` skill.
6. Code written for the spike is throwaway. Do not merge it as it is: either delete it or
   bring it up to the project's rules, with tests, in its own ticket.
