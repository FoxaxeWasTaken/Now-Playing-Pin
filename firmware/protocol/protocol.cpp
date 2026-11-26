#include "protocol.h"

namespace npp::protocol {

namespace {

constexpr uint8_t kOpen = 1;
constexpr uint8_t kEnd = 2;

uint16_t read16(const uint8_t* p) { return static_cast<uint16_t>(p[0] | p[1] << 8); }

uint32_t read32(const uint8_t* p) {
    return static_cast<uint32_t>(p[0]) | static_cast<uint32_t>(p[1]) << 8 |
           static_cast<uint32_t>(p[2]) << 16 | static_cast<uint32_t>(p[3]) << 24;
}

uint8_t* write16(uint8_t* p, uint16_t value) {
    p[0] = static_cast<uint8_t>(value);
    p[1] = static_cast<uint8_t>(value >> 8);
    return p + 2;
}

uint8_t* write32(uint8_t* p, uint32_t value) {
    for (int i = 0; i < 4; ++i) p[i] = static_cast<uint8_t>(value >> (8 * i));
    return p + 4;
}

bool is_state(uint8_t value) { return value <= static_cast<uint8_t>(PlaybackState::paused); }

bool is_link_byte(uint8_t value) { return value >= 0x21 && value <= 0x7E; }  // printable ASCII

}  // namespace

// The checks follow the order of the refusal reasons: version, then form, then image size,
// then link (docs/protocol.md, "Control").
Error decode_track(const uint8_t* bytes, size_t size, Track& out) {
    if (size < 1) return Error::malformed;
    if (bytes[0] != kVersion) return Error::unknown_version;
    if (size < 2) return Error::malformed;
    Track track;
    if (bytes[1] == kEnd) {
        if (size != 8) return Error::malformed;
        track.type = Track::Type::end;
        track.counter = read16(bytes + 2);
        track.identity = read32(bytes + 4);
        out = track;
        return Error::none;
    }
    if (bytes[1] != kOpen || size < 10 || !is_state(bytes[4]) || size != 10u + bytes[9]) {
        return Error::malformed;
    }
    track.type = Track::Type::open;
    track.counter = read16(bytes + 2);
    track.state = static_cast<PlaybackState>(bytes[4]);
    track.cover_width = bytes[5];
    track.cover_height = bytes[6];
    track.text_width = bytes[7];
    track.text_height = bytes[8];
    track.link_length = bytes[9];
    if (track.cover_width != kCoverWidth || track.cover_height != kCoverHeight ||
        track.text_width != kTextWidth || track.text_height != kTextHeight) {
        return Error::image_size;
    }
    if (track.link_length > kMaxLink) return Error::invalid_link;
    for (size_t i = 0; i < track.link_length; ++i) {
        if (!is_link_byte(bytes[10 + i])) return Error::invalid_link;
        track.link[i] = static_cast<char>(bytes[10 + i]);
    }
    out = track;
    return Error::none;
}

Error decode_image_packet(const uint8_t* bytes, size_t size, size_t image_size, ImagePacket& out) {
    if (size <= kPacketHeader) return Error::malformed;
    const uint16_t offset = read16(bytes + 2);
    if (offset + (size - kPacketHeader) > image_size) return Error::beyond_image;
    out.counter = read16(bytes);
    out.offset = offset;
    out.data = bytes + kPacketHeader;
    out.size = size - kPacketHeader;
    return Error::none;
}

Error decode_state(const uint8_t* bytes, size_t size, PlaybackState& out) {
    if (size < 1) return Error::malformed;
    if (bytes[0] != kVersion) return Error::unknown_version;
    if (size != 2 || !is_state(bytes[1])) return Error::malformed;
    out = static_cast<PlaybackState>(bytes[1]);
    return Error::none;
}

Error decode_control(const uint8_t* bytes, size_t size, Control& out) {
    if (size < 1) return Error::malformed;
    Control control;
    switch (bytes[0]) {
        case static_cast<uint8_t>(Control::Type::hello): {
            if (size < 2) return Error::malformed;
            if (bytes[1] != kVersion) return Error::unknown_version;
            if (size != 10 || bytes[5] > 1) return Error::malformed;
            control.type = Control::Type::hello;
            for (int i = 0; i < 3; ++i) control.firmware[i] = bytes[2 + i];
            control.held = bytes[5] == 1;
            control.identity = read32(bytes + 6);
            if (!control.held && control.identity != 0) return Error::malformed;
            break;
        }
        case static_cast<uint8_t>(Control::Type::acknowledgement):
        case static_cast<uint8_t>(Control::Type::resend_request):
            if (size != 3) return Error::malformed;
            control.type = static_cast<Control::Type>(bytes[0]);
            control.counter = read16(bytes + 1);
            break;
        case static_cast<uint8_t>(Control::Type::refusal):
            if (size != 2 || bytes[1] < static_cast<uint8_t>(Error::unknown_version) ||
                bytes[1] > static_cast<uint8_t>(Error::invalid_link)) {
                return Error::malformed;
            }
            control.type = Control::Type::refusal;
            control.reason = static_cast<Error>(bytes[1]);
            break;
        default:
            return Error::malformed;
    }
    out = control;
    return Error::none;
}

size_t encode_track(const Track& message, uint8_t* out) {
    uint8_t* p = out;
    *p++ = kVersion;
    if (message.type == Track::Type::end) {
        *p++ = kEnd;
        p = write16(p, message.counter);
        p = write32(p, message.identity);
        return static_cast<size_t>(p - out);
    }
    if (message.link_length > kMaxLink) return 0;
    *p++ = kOpen;
    p = write16(p, message.counter);
    *p++ = static_cast<uint8_t>(message.state);
    *p++ = message.cover_width;
    *p++ = message.cover_height;
    *p++ = message.text_width;
    *p++ = message.text_height;
    *p++ = message.link_length;
    for (size_t i = 0; i < message.link_length; ++i) *p++ = static_cast<uint8_t>(message.link[i]);
    return static_cast<size_t>(p - out);
}

size_t encode_image_packet(const ImagePacket& packet, uint8_t* out) {
    uint8_t* p = write16(out, packet.counter);
    p = write16(p, packet.offset);
    for (size_t i = 0; i < packet.size; ++i) *p++ = packet.data[i];
    return static_cast<size_t>(p - out);
}

size_t encode_state(PlaybackState state, uint8_t* out) {
    out[0] = kVersion;
    out[1] = static_cast<uint8_t>(state);
    return 2;
}

size_t encode_control(const Control& message, uint8_t* out) {
    uint8_t* p = out;
    *p++ = static_cast<uint8_t>(message.type);
    switch (message.type) {
        case Control::Type::hello:
            *p++ = kVersion;
            for (uint8_t part : message.firmware) *p++ = part;
            *p++ = message.held ? 1 : 0;
            p = write32(p, message.identity);
            break;
        case Control::Type::acknowledgement:
        case Control::Type::resend_request:
            p = write16(p, message.counter);
            break;
        case Control::Type::refusal:
            *p++ = static_cast<uint8_t>(message.reason);
            break;
    }
    return static_cast<size_t>(p - out);
}

uint32_t crc32(uint32_t crc, const uint8_t* bytes, size_t size) {
    crc = ~crc;
    for (size_t i = 0; i < size; ++i) {
        crc ^= bytes[i];
        for (int bit = 0; bit < 8; ++bit) crc = (crc >> 1) ^ (0xEDB88320u & (0u - (crc & 1u)));
    }
    return ~crc;
}

}  // namespace npp::protocol
