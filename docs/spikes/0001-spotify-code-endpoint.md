# Spike 0001: Spotify Code endpoint and bar extraction

- Status: Planned
- Needs the pin: no
- Time box: half a day (suggested)
- Feeds: [ADR-0007](../adr/0007-qr-share-view-spotify-code-later.md),
  [android-app.md](../android-app.md), test data

The Spotify Code is not part of V1 ([ADR-0010](../adr/0010-v1-scope.md)). This spike is
short and can run at any time: its answer decides whether the feature is worth keeping on
the "Later" list.

## Question

Does `https://scannables.scdn.co/uri/plain/svg/FFFFFF/black/640/<uri>` return an SVG from
which the 23 bar heights can be extracted reliably, and can a code redrawn from 20 values
be read by the Spotify app?

## Why it matters

The endpoint is unofficial. The whole Spotify Code feature, the 8-byte format reserved in
the protocol and the claim that bars 1, 12 and 23 are fixed all rest on it.

## Method

1. Fetch the SVG for about 20 different tracks with a small script.
2. Look at the structure: how the bars are described, whether there are exactly 23, how many
   distinct heights exist.
3. Check that bars 1, 12 and 23 have the same height in every sample, and note which.
4. Extract the levels, redraw a code from the 20 values plus the 3 fixed bars, show it on a
   monitor and scan it with the Spotify app.
5. Note anything that matters for the app: required headers, response size, errors, signs of
   rate limiting.
6. Write down the bar heights of a few samples as numbers, to build test data from. The
   fetched files themselves are not committed: they contain Spotify's logo.

## Done when

- The answer is yes or no, with the samples to show it.
- If yes: the extraction rule is written down, and a redrawn code was scanned successfully.

## Notes

During a documentation review on 2026-11-19, the endpoint was queried for two tracks and
was reported to answer as described: 23 bars, 8 heights, the first and last bar at the
lowest height and the 12th at the highest. This is an informal check, not the result of
this spike.

## Result

Not run.

## Outcome

To be written. Expected: the extraction rule in android-app.md, numeric test data for the
parser, and a new ADR if the result changes ADR-0007.
