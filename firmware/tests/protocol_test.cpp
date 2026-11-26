// Contract tests of the protocol: every vector of tools/vectors/ in both directions
// (docs/testing.md, "Level 2: contract tests"), then a short fuzzing run of the decoders.
#include "protocol.h"

#include <doctest/doctest.h>

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <map>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace npp::protocol;

namespace {

using Fields = std::map<std::string, std::string>;
using Bytes = std::vector<uint8_t>;

// Reads a vector file: blocks of "key: value" lines, separated by blank lines.
std::vector<Fields> load(const std::string& file) {
    std::ifstream in(std::string(NPP_VECTORS_DIR) + "/" + file);
    REQUIRE(in.good());
    std::vector<Fields> vectors(1);
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) {
            if (!vectors.back().empty()) vectors.emplace_back();
            continue;
        }
        if (line[0] == '#') continue;
        const auto colon = line.find(':');
        REQUIRE(colon != std::string::npos);
        std::string value = line.substr(colon + 1);
        value.erase(0, value.find_first_not_of(' '));
        value.erase(value.find_last_not_of(' ') + 1);
        vectors.back()[line.substr(0, colon)] = value;
    }
    if (vectors.back().empty()) vectors.pop_back();
    REQUIRE(!vectors.empty());
    return vectors;
}

Bytes hex(const std::string& text) {
    Bytes bytes;
    std::istringstream in(text);
    std::string pair;
    while (in >> pair) bytes.push_back(static_cast<uint8_t>(std::stoul(pair, nullptr, 16)));
    return bytes;
}

uint32_t number(const std::string& text) {
    const bool is_hex = text.rfind("0x", 0) == 0;
    return static_cast<uint32_t>(
        std::stoul(is_hex ? text.substr(2) : text, nullptr, is_hex ? 16 : 10));
}

const std::map<std::string, Error> kErrors = {
    {"unknown_version", Error::unknown_version}, {"malformed", Error::malformed},
    {"image_size", Error::image_size},           {"invalid_link", Error::invalid_link},
    {"beyond_image", Error::beyond_image},
};

const std::map<std::string, PlaybackState> kStates = {
    {"nothing_playing", PlaybackState::nothing_playing},
    {"playing", PlaybackState::playing},
    {"paused", PlaybackState::paused},
};

Track track_from(const Fields& v) {
    Track t;
    t.counter = static_cast<uint16_t>(number(v.at("counter")));
    if (v.at("message") == "end") {
        t.type = Track::Type::end;
        t.identity = number(v.at("identity"));
        return t;
    }
    REQUIRE(v.at("message") == "open");
    t.state = kStates.at(v.at("state"));
    t.cover_width = static_cast<uint8_t>(number(v.at("cover_width")));
    t.cover_height = static_cast<uint8_t>(number(v.at("cover_height")));
    t.text_width = static_cast<uint8_t>(number(v.at("text_width")));
    t.text_height = static_cast<uint8_t>(number(v.at("text_height")));
    const std::string& link = v.at("link");
    REQUIRE(link.size() <= kMaxLink);
    t.link_length = static_cast<uint8_t>(link.size());
    link.copy(t.link, link.size());
    return t;
}

Control control_from(const Fields& v) {
    Control c;
    const std::string& message = v.at("message");
    if (message == "hello") {
        c.type = Control::Type::hello;
        unsigned major = 0, minor = 0, patch = 0;
        REQUIRE(std::sscanf(v.at("firmware").c_str(), "%u.%u.%u", &major, &minor, &patch) == 3);
        c.firmware[0] = static_cast<uint8_t>(major);
        c.firmware[1] = static_cast<uint8_t>(minor);
        c.firmware[2] = static_cast<uint8_t>(patch);
        c.held = number(v.at("held")) == 1;
        c.identity = number(v.at("identity"));
    } else if (message == "acknowledgement" || message == "resend_request") {
        c.type = message == "acknowledgement" ? Control::Type::acknowledgement
                                              : Control::Type::resend_request;
        c.counter = static_cast<uint16_t>(number(v.at("counter")));
    } else {
        REQUIRE(message == "refusal");
        c.type = Control::Type::refusal;
        c.reason = kErrors.at(v.at("reason"));
    }
    return c;
}

void check_same(const Track& got, const Track& want) {
    REQUIRE(got.type == want.type);
    CHECK(got.counter == want.counter);
    if (want.type == Track::Type::end) {
        CHECK(got.identity == want.identity);
        return;
    }
    CHECK(got.state == want.state);
    CHECK(got.cover_width == want.cover_width);
    CHECK(got.cover_height == want.cover_height);
    CHECK(got.text_width == want.text_width);
    CHECK(got.text_height == want.text_height);
    CHECK(std::string(got.link, got.link_length) == std::string(want.link, want.link_length));
}

void check_same(const Control& got, const Control& want) {
    REQUIRE(got.type == want.type);
    switch (want.type) {
        case Control::Type::hello:
            CHECK(Bytes(got.firmware, got.firmware + 3) == Bytes(want.firmware, want.firmware + 3));
            CHECK(got.held == want.held);
            CHECK(got.identity == want.identity);
            break;
        case Control::Type::acknowledgement:
        case Control::Type::resend_request:
            CHECK(got.counter == want.counter);
            break;
        case Control::Type::refusal:
            CHECK(got.reason == want.reason);
            break;
    }
}

// The version of every valid vector is the one this code speaks.
void check_version(const Fields& v) {
    if (v.count("version")) CHECK(number(v.at("version")) == kVersion);
}

}  // namespace

TEST_CASE("Track vectors, both directions") {
    for (const Fields& v : load("track.txt")) {
        const std::string name = v.at("name");
        CAPTURE(name);
        const Bytes bytes = hex(v.at("bytes"));
        Track decoded;
        const Error error = decode_track(bytes.data(), bytes.size(), decoded);
        if (v.count("error")) {
            CHECK(error == kErrors.at(v.at("error")));
            continue;
        }
        REQUIRE(error == Error::none);
        check_version(v);
        const Track expected = track_from(v);
        check_same(decoded, expected);
        uint8_t out[kMaxMessage];
        CHECK(Bytes(out, out + encode_track(expected, out)) == bytes);
    }
}

TEST_CASE("image packet vectors, both directions") {
    for (const Fields& v : load("image.txt")) {
        const std::string name = v.at("name");
        CAPTURE(name);
        const Bytes bytes = hex(v.at("bytes"));
        ImagePacket decoded;
        const Error error =
            decode_image_packet(bytes.data(), bytes.size(), number(v.at("image_size")), decoded);
        if (v.count("error")) {
            CHECK(error == kErrors.at(v.at("error")));
            continue;
        }
        REQUIRE(error == Error::none);
        REQUIRE(v.at("message") == "packet");
        const Bytes data = hex(v.at("data"));
        CHECK(decoded.counter == number(v.at("counter")));
        CHECK(decoded.offset == number(v.at("offset")));
        CHECK(Bytes(decoded.data, decoded.data + decoded.size) == data);
        ImagePacket expected;
        expected.counter = static_cast<uint16_t>(number(v.at("counter")));
        expected.offset = static_cast<uint16_t>(number(v.at("offset")));
        expected.data = data.data();
        expected.size = data.size();
        Bytes out(kPacketHeader + data.size());
        out.resize(encode_image_packet(expected, out.data()));
        CHECK(out == bytes);
    }
}

TEST_CASE("State vectors, both directions") {
    for (const Fields& v : load("state.txt")) {
        const std::string name = v.at("name");
        CAPTURE(name);
        const Bytes bytes = hex(v.at("bytes"));
        PlaybackState decoded = PlaybackState::nothing_playing;
        const Error error = decode_state(bytes.data(), bytes.size(), decoded);
        if (v.count("error")) {
            CHECK(error == kErrors.at(v.at("error")));
            continue;
        }
        REQUIRE(error == Error::none);
        REQUIRE(v.at("message") == "state");
        check_version(v);
        const PlaybackState expected = kStates.at(v.at("state"));
        CHECK(decoded == expected);
        uint8_t out[kMaxMessage];
        CHECK(Bytes(out, out + encode_state(expected, out)) == bytes);
    }
}

TEST_CASE("Control vectors, both directions") {
    for (const Fields& v : load("control.txt")) {
        const std::string name = v.at("name");
        CAPTURE(name);
        const Bytes bytes = hex(v.at("bytes"));
        Control decoded;
        const Error error = decode_control(bytes.data(), bytes.size(), decoded);
        if (v.count("error")) {
            CHECK(error == kErrors.at(v.at("error")));
            continue;
        }
        REQUIRE(error == Error::none);
        check_version(v);
        const Control expected = control_from(v);
        check_same(decoded, expected);
        uint8_t out[kMaxMessage];
        CHECK(Bytes(out, out + encode_control(expected, out)) == bytes);
    }
}

TEST_CASE("identity vectors: the CRC-32 of cover, text and link") {
    for (const Fields& v : load("identity.txt")) {
        const std::string name = v.at("name");
        CAPTURE(name);
        const Bytes cover = hex(v.at("cover"));
        const Bytes text = hex(v.at("text"));
        const std::string& link = v.at("link");
        uint32_t crc = crc32(0, cover.data(), cover.size());
        crc = crc32(crc, text.data(), text.size());
        crc = crc32(crc, reinterpret_cast<const uint8_t*>(link.data()), link.size());
        if (v.count("error")) {
            REQUIRE(v.at("error") == "wrong_crc");
            CHECK(crc != number(v.at("identity")));
        } else {
            REQUIRE(v.at("message") == "identity");
            CHECK(crc == number(v.at("identity")));
        }
    }
}

// Random and mutated inputs, under the sanitizers: no decoder may read out of bounds, and
// whatever a decoder accepts must encode back to the very same bytes.
TEST_CASE("the decoders survive random bytes and accept only what they can encode back") {
    std::vector<Bytes> seeds;
    for (const char* file : {"track.txt", "image.txt", "state.txt", "control.txt"}) {
        for (const Fields& v : load(file)) seeds.push_back(hex(v.at("bytes")));
    }
    std::mt19937 random(20261126);  // fixed seed, so that a failure can be replayed
    auto below = [&random](size_t n) { return static_cast<size_t>(random() % n); };
    for (int round = 0; round < 200000; ++round) {
        Bytes bytes;
        if (round % 2 == 0) {
            bytes.resize(below(kMaxMessage + 12));
            for (uint8_t& b : bytes) b = static_cast<uint8_t>(random());
        } else {
            bytes = seeds[below(seeds.size())];
            for (size_t flips = 1 + below(3); flips > 0 && !bytes.empty(); --flips) {
                bytes[below(bytes.size())] ^= static_cast<uint8_t>(1u << below(8));
            }
            if (below(4) == 0) bytes.resize(below(bytes.size() + 2));
        }
        CAPTURE(round);
        uint8_t out[kMaxMessage + 12];
        Track track;
        if (decode_track(bytes.data(), bytes.size(), track) == Error::none) {
            REQUIRE(Bytes(out, out + encode_track(track, out)) == bytes);
        }
        ImagePacket packet;
        if (decode_image_packet(bytes.data(), bytes.size(), 1952, packet) == Error::none) {
            REQUIRE(Bytes(out, out + encode_image_packet(packet, out)) == bytes);
        }
        PlaybackState state;
        if (decode_state(bytes.data(), bytes.size(), state) == Error::none) {
            REQUIRE(Bytes(out, out + encode_state(state, out)) == bytes);
        }
        Control control;
        if (decode_control(bytes.data(), bytes.size(), control) == Error::none) {
            REQUIRE(Bytes(out, out + encode_control(control, out)) == bytes);
        }
    }
}
