# Spike 0002: Metadata event timing

- Status: Planned
- Needs the pin: no
- Time box: one day, most of it listening (suggested)
- Feeds: [android-app.md](../android-app.md)

The logging code is written as a normal task of the [roadmap](../roadmap.md) and is kept.
This spike is the measurement campaign run with it.

## Question

When a track changes, in what order and with what delay do the title, the cover and the
Spotify broadcast arrive? Is a 500 ms wait enough to send a complete bundle once?

## Why it matters

The pin refreshes once per track. If the app sends too early, the cover is missing or
belongs to the previous track. If it waits too long, the pin feels slow. The 500 ms value is
a guess.

## Method

1. With the logging app, record every MediaSession callback and every Spotify broadcast with
   a timestamp, and every metadata key each event carries.
2. Log about 50 track changes on Spotify: normal end of track, manual skip, several fast
   skips, a seek, a different album, the same album.
3. Do the same on at least two other music apps. Note where each app puts the cover.
4. Note what the Spotify broadcast carries for content that is not a track (podcast,
   advertisement), and what happens when "Device Broadcast Status" is off.
5. Note what opens a media session without being music: a video, a voice message.

The raw logs are a listening history. They stay on the phone and are not committed. What is
written in this document is delays and counts, with the tracks replaced by numbers.

## Done when

- The spread of delays between the first event and the last useful one is known per app.
- A wait value is chosen and justified, or the fixed wait is replaced by another rule.
- The merge rules between the two sources are confirmed or corrected.
- There is enough evidence to write the list of apps shown on the pin
  ([ADR-0015](../adr/0015-only-chosen-apps-are-shown.md)).

## Result

Not run.

## Outcome

To be written. Expected: updated values and rules in android-app.md, and test cases for the
debounce and merge logic.
