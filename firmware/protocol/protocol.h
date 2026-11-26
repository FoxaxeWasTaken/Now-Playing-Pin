#pragma once
// Encoding and decoding of the BLE protocol messages, draft v0 (docs/protocol.md).
// Portable: no Arduino, no allocation, no exceptions. The pin decodes what the phone writes
// and encodes Control; the other directions exist so that every test vector runs both ways.
#include <cstddef>
#include <cstdint>

namespace npp::protocol {

constexpr uint8_t kVersion = 0;
constexpr size_t kMaxLink = 78;
constexpr size_t kMaxMessage = 10 + kMaxLink;  // the longest Open
constexpr size_t kPacketHeader = 4;

// The only image sizes the pin places in V1 (docs/display-spec.md).
constexpr uint8_t kCoverWidth = 122;
constexpr uint8_t kCoverHeight = 122;
constexpr uint8_t kTextWidth = 128;
constexpr uint8_t kTextHeight = 122;

// The first four values are the refusal reasons sent on Control, in their order of priority.
enum class Error : uint8_t {
    none = 0,
    unknown_version = 1,
    malformed = 2,
    image_size = 3,
    invalid_link = 4,
    beyond_image,  // an image packet past the end of its image: dropped, not refused
};

enum class PlaybackState : uint8_t { nothing_playing = 0, playing = 1, paused = 2 };

// A message written on Track: Open or End. Only the fields of its type are meaningful.
struct Track {
    enum class Type : uint8_t { open = 1, end = 2 };
    Type type = Type::open;
    uint16_t counter = 0;
    // Open
    PlaybackState state = PlaybackState::nothing_playing;
    uint8_t cover_width = 0;
    uint8_t cover_height = 0;
    uint8_t text_width = 0;
    uint8_t text_height = 0;
    uint8_t link_length = 0;
    char link[kMaxLink] = {};
    // End
    uint32_t identity = 0;
};

struct ImagePacket {
    uint16_t counter = 0;
    uint16_t offset = 0;
    const uint8_t* data = nullptr;  // points into the bytes that were decoded
    size_t size = 0;
};

// A message notified on Control. Only the fields of its type are meaningful.
struct Control {
    enum class Type : uint8_t { hello = 1, acknowledgement = 2, resend_request = 3, refusal = 4 };
    Type type = Type::hello;
    // Hello
    uint8_t firmware[3] = {};  // major, minor, patch
    bool held = false;
    uint32_t identity = 0;
    // Acknowledgement and resend request
    uint16_t counter = 0;
    // Refusal
    Error reason = Error::none;
};

// Each decoder fills `out` only when it returns Error::none.
Error decode_track(const uint8_t* bytes, size_t size, Track& out);
Error decode_image_packet(const uint8_t* bytes, size_t size, size_t image_size, ImagePacket& out);
Error decode_state(const uint8_t* bytes, size_t size, PlaybackState& out);
Error decode_control(const uint8_t* bytes, size_t size, Control& out);

// Each encoder writes valid fields into `out` and returns the number of bytes written.
// `out` holds kMaxMessage bytes, or kPacketHeader plus the data for an image packet.
size_t encode_track(const Track& message, uint8_t* out);
size_t encode_image_packet(const ImagePacket& packet, uint8_t* out);
size_t encode_state(PlaybackState state, uint8_t* out);
size_t encode_control(const Control& message, uint8_t* out);

// CRC-32/ISO-HDLC, the one of zlib: crc32(crc32(0, a), b) is the CRC of a then b.
uint32_t crc32(uint32_t crc, const uint8_t* bytes, size_t size);

}  // namespace npp::protocol
