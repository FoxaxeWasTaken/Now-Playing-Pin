# ADR-0007: QR code as the share view, Spotify Code later

- Status: Accepted
- Date: 2026-11-19 (recorded; the decision is older, the wording follows ADR-0010)
- Related: [firmware.md](../firmware.md), [android-app.md](../android-app.md),
  [ADR-0010](0010-v1-scope.md),
  [spike 0001](../spikes/0001-spotify-code-endpoint.md),
  [spike 0005](../spikes/0005-qr-size-and-scannability.md)

## Context

The share screen should let someone open the current track. A Spotify Code looks best and
is read by the Spotify app, but the only known way to get one is an unofficial endpoint
(`scannables.scdn.co`) that may disappear. The project must not depend on a network call to
work.

## Decision

- The share link is always built by the phone without any network: a link to the track when
  its id is known, otherwise a search link built from title and artist.
- The pin generates a QR code from that link. This is the share view, and the only one in V1.
- The Spotify Code is a later addition. When it is built: the phone fetches it when the
  network is available, extracts the bar levels and caches them; the pin shows it instead of
  the QR code when the bars were received.

## Alternatives

- Spotify Code only. Rejected: it depends on the unofficial endpoint and on the network, and
  does nothing for other music apps.

## Consequences

- The pin embeds a QR code library.
- In V1 the app needs no network permission.
- The protocol reserves a place for the bar levels (8 bytes) without using it in V1.
- The Spotify logo and Spotify Codes are Spotify's marks. No logo, no fetched code and no
  real cover art is committed to this repository. How the logo reaches the pin is decided
  when the feature is built.
- If the endpoint changes or disappears, only the Spotify Code is lost.
