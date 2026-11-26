# Android companion app

Status: design. The project builds an empty app. The only code is the encoding and decoding
of the protocol messages, in the `protocol` package, which has no Android import. Kotlin. This
document describes V1
([ADR-0010](adr/0010-v1-scope.md)).

The app does all the work: it watches playback, prepares the images and pushes them to the
pin. Every data source is event driven. Nothing polls.

## Target and distribution

- The app is installed by hand, not through a store
  ([ADR-0011](adr/0011-manual-install.md)).
- Minimum Android version: Android 13 (API 33), which keeps the permission model simple.
- The app targets API 37, so the rules of Android 14 and later apply.
- Built with Gradle and the Android Gradle plugin, which compiles Kotlin itself. The app has
  no library dependency.

Consequence of installing by hand: Android treats notification access as a restricted
setting for such apps. The user has to allow restricted settings for the app once, in the
system settings, before notification access can be granted **[to verify on the phone]**.

## Data sources

### MediaSession

- `NotificationListenerService` plus `MediaSessionManager.getActiveSessions(componentName)`.
  The list comes back in priority order.
- `MediaController.Callback.onMetadataChanged` and `onPlaybackStateChanged`.
- `OnActiveSessionsChangedListener` to follow sessions that appear and disappear.

This gives title, artist and playback state for most music apps. The cover is read from
`METADATA_KEY_ALBUM_ART`, then `METADATA_KEY_ART`. Some apps only give a URI or put the cover
in their notification: what each app really provides is part of
[spike 0002](spikes/0002-metadata-event-timing.md).

### Spotify broadcast

- Broadcast `com.spotify.music.metadatachanged`, extra `id` = `spotify:track:<id>`.
- Requires "Device Broadcast Status" to be enabled in Spotify's settings. It is reported as
  off by default and cannot be turned on by another app.
- The receiver is **registered in code**, inside the notification listener service, and
  declared as exported, which Android 14 and later require for a broadcast coming from
  another app. The listener runs whether or not the pin is in range, so the id of the track
  already playing is known when the pin connects.

The id is therefore optional. An absent id is a normal case, not an error: the app falls
back to a search link. The id is not always a track (podcast episodes), so the link is built
from the kind of id received, never by assuming a track.

Any app on the phone can send this broadcast, so it is not trusted:

- The id is accepted only if it has the exact shape of a Spotify track or episode id, and
  only while the session being shown belongs to the Spotify app.
- The title, the artist and the cover never come from the broadcast. They come from the
  media session.
- A link always starts with a fixed scheme and host written in the code. The title and the
  artist are encoded before they are added, wherever the site expects them, and never pasted
  as they are into the address.

What remains possible: another app could make the QR code point to a different Spotify
track. This is accepted.

## Which apps are shown

Only apps on a list of chosen apps ([ADR-0015](adr/0015-only-chosen-apps-are-shown.md)). The
pin is worn in public, and a media session is not always music: a podcast, a video or a
voice message can open one. A session from an app that is not on the list is ignored, as if
nothing was playing.

V1 has no settings screen, so the list is a constant in the code. In V1 it holds Spotify
only.

## Pipeline for one track

1. **Debounce.** Wait about 500 ms after `onMetadataChanged`, because the cover sometimes
   arrives after the title. Merge both sources. The session used is the one that is playing,
   among the chosen apps, Spotify first.
2. **Deduplicate.** Compare the result with the last bundle sent. If title, artist, cover and
   link are the same, nothing is sent: playback callbacks also fire on a seek, and each
   useless bundle costs the pin a transfer and a refresh.
3. **Cover.** Crop to a square, scale to 122 × 122, adjust levels, Atkinson dithering, 1 bit
   per pixel. When the track has no cover, the app draws a placeholder: the pin always
   receives a cover image.
4. **Text.** Title and artist are drawn by the phone into a 128 × 122, 1 bit per pixel bitmap.
   The app bundles one font for the scripts it covers and a monochrome emoji font; other
   scripts fall back to the device's fonts. Every script and every emoji works without a
   font on the pin.
5. **Share link.** A link to the track when Spotify gave an id, otherwise a search link built
   from title and artist. 78 bytes at most. The links and the rule that shortens a long
   title are in [protocol.md](protocol.md), "Share link".
6. **Send over BLE**, as described in [protocol.md](protocol.md).

The image processing steps, the fonts and every size are in
[display-spec.md](display-spec.md).

A change of playback state with the same track sends a State message, not a bundle.

## Connection lifecycle

The sequence on the wire is in [protocol.md](protocol.md). On the app side:

1. **Association**, once, through `CompanionDeviceManager`, while the pin's pairing window is
   open. The app then asks for the bond and waits until it is established; the user types
   the passkey shown on the pin.
2. **Presence.** The app asks the system to observe the associated device, with the call
   that is current for the targeted API level. The system starts the app's service when the
   pin is in range, including after a restart of the phone.
3. **Connection.** The service connects, waits for the negotiated MTU, discovers the services
   and subscribes to Control.
4. **Synchronisation.** On the pin's hello, the app sends the current bundle if the pin does
   not already hold it, then the playback state in every case.
5. **Link lost.** As soon as the link drops, the service leaves a connection request pending
   with the system, which reconnects when the pin is back. It does not scan in a loop. The
   presence observation of step 2 is only there to restart the app if it was stopped: after
   a short loss of link it usually reports nothing.
6. **Bond lost.** If the pin's bond was erased, the app says so and asks for a new
   association. It does not retry silently. The phone still holds its old keys, which the
   app cannot delete: the user removes the pin in the phone's Bluetooth settings before
   pairing again.

Writes go out one at a time: the app waits for the write callback before sending the next
packet, including for writes without response.

## Manifest

Expected content, from the Android documentation, to confirm when the app is scaffolded:

| Entry | Why |
| --- | --- |
| `BLUETOOTH_CONNECT` | Talk to the bonded pin |
| `FOREGROUND_SERVICE`, `FOREGROUND_SERVICE_CONNECTED_DEVICE` | Foreground service of type `connectedDevice` |
| `POST_NOTIFICATIONS` | The service's own notification |
| `REQUEST_COMPANION_START_FOREGROUND_SERVICES_FROM_BACKGROUND` | Start the service when the pin comes in range |
| `REQUEST_OBSERVE_COMPANION_DEVICE_PRESENCE`, with a `CompanionDeviceService` | Be told when the pin is in range |
| `REQUEST_IGNORE_BATTERY_OPTIMIZATIONS` | Ask to be excluded from battery optimisation |
| `uses-feature android.software.companion_device_setup` | Companion association |
| The notification listener service, with `BIND_NOTIFICATION_LISTENER_SERVICE` | Read media sessions |

- `BLUETOOTH_SCAN` is probably not needed, because the association goes through the system.
- `INTERNET` is **not requested in V1**. It is only needed for the Spotify Code, which is
  planned for later. CI fails if the built app declares it.
- Backup of the app's data is turned off: no cloud backup, and no copy to a new phone
  (`allowBackup="false"` and data extraction rules that exclude everything).
- Notification access is granted by the user in the system settings.

## Privacy

- Notification access lets an app see every notification. This app only reads media
  sessions, and keeps nothing.
- In V1 the app has no network permission: nothing leaves the phone except what goes to the
  pin.
- Event logging exists only in debug builds. A log of what was played is a listening
  history: it stays on the phone and is never committed.
- What goes to the pin is shown in public, which is why only chosen apps are shown.

## User interface

Minimal in V1:

- Association with the pin.
- Connection state.
- Pin battery level.
- Preview: the cover bitmap and the text bitmap side by side, as they are sent.

No settings screen in V1.

## Later

- **Spotify Code.** GET
  `https://scannables.scdn.co/uri/plain/svg/FFFFFF/black/640/<uri>`, extract the 23 bar
  heights (8 levels), keep the 20 that carry data (bars 1, 12 and 23 are fixed reference
  bars), 3 bits each, cache by URI. The endpoint is unofficial and may disappear
  ([spike 0001](spikes/0001-spotify-code-endpoint.md)). The bundle is sent without waiting
  for it; the bars follow when they are known.
- **Media control.** Button presses arrive from the pin as notifications and are applied to
  the active MediaSession, after checking that the session accepts the action.
- **Settings**: rotation, inversion, discreet mode, custom sleep image, choice of apps.
