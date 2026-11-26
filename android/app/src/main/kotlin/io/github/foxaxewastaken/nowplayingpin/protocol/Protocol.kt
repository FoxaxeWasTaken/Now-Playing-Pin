package io.github.foxaxewastaken.nowplayingpin.protocol

// Encoding and decoding of the BLE protocol messages, draft v0 (docs/protocol.md).
// Plain Kotlin with no Android import, tested against the vectors of tools/vectors/, like
// firmware/protocol/ on the pin.

const val PROTOCOL_VERSION = 0
const val MAX_LINK = 78
const val PACKET_HEADER = 4

// The only image sizes the pin places in V1 (docs/display-spec.md).
const val COVER_WIDTH = 122
const val COVER_HEIGHT = 122
const val TEXT_WIDTH = 128
const val TEXT_HEIGHT = 122

enum class PlaybackState(
    val code: Int,
) {
    NOTHING_PLAYING(0),
    PLAYING(1),
    PAUSED(2),
}

// The refusal reasons sent on Control, in their order of priority, then the errors the pin
// keeps to itself.
enum class ProtocolError(
    val reason: Int?,
) {
    UNKNOWN_VERSION(1),
    MALFORMED(2),
    IMAGE_SIZE(3),
    INVALID_LINK(4),
    BEYOND_IMAGE(null),
}

class ProtocolException(
    val error: ProtocolError,
) : Exception(error.name)

sealed interface TrackMessage {
    val counter: Int
}

data class Open(
    override val counter: Int,
    val state: PlaybackState,
    val coverWidth: Int,
    val coverHeight: Int,
    val textWidth: Int,
    val textHeight: Int,
    val link: String,
) : TrackMessage

data class End(
    override val counter: Int,
    val identity: Long,
) : TrackMessage

class ImagePacket(
    val counter: Int,
    val offset: Int,
    val data: ByteArray,
)

sealed interface ControlMessage

data class Hello(
    val firmware: List<Int>,
    val held: Boolean,
    val identity: Long,
) : ControlMessage

data class Acknowledgement(
    val counter: Int,
) : ControlMessage

data class ResendRequest(
    val counter: Int,
) : ControlMessage

data class Refusal(
    val reason: ProtocolError,
) : ControlMessage

private const val OPEN = 1
private const val END = 2
private const val HELLO = 1
private const val ACKNOWLEDGEMENT = 2
private const val RESEND_REQUEST = 3
private const val REFUSAL = 4

// The encoders take fields built by the app and refuse values the protocol cannot carry.
fun encodeTrack(message: TrackMessage): ByteArray =
    when (message) {
        is Open -> {
            require(message.link.length <= MAX_LINK && message.link.all { isLinkChar(it.code) }) {
                "the link must be printable ASCII, $MAX_LINK bytes at most"
            }
            bytes(
                PROTOCOL_VERSION,
                OPEN,
                *u16(message.counter),
                message.state.code,
                *u8s(message.coverWidth, message.coverHeight, message.textWidth, message.textHeight),
                message.link.length,
            ) + message.link.toByteArray(Charsets.US_ASCII)
        }

        is End -> {
            bytes(PROTOCOL_VERSION, END, *u16(message.counter), *u32(message.identity))
        }
    }

// The checks follow the order of the refusal reasons: version, then form, then image size,
// then link (docs/protocol.md, "Control").
fun decodeTrack(bytes: ByteArray): TrackMessage {
    check(bytes.isNotEmpty())
    if (bytes.u8(0) != PROTOCOL_VERSION) fail(ProtocolError.UNKNOWN_VERSION)
    check(bytes.size >= 2)
    if (bytes.u8(1) == END) {
        check(bytes.size == 8)
        return End(bytes.u16(2), bytes.u32(4))
    }
    check(bytes.u8(1) == OPEN && bytes.size >= 10 && isState(bytes.u8(4)) && bytes.size == 10 + bytes.u8(9))
    val open =
        Open(
            counter = bytes.u16(2),
            state = PlaybackState.entries.first { it.code == bytes.u8(4) },
            coverWidth = bytes.u8(5),
            coverHeight = bytes.u8(6),
            textWidth = bytes.u8(7),
            textHeight = bytes.u8(8),
            link = String(bytes, 10, bytes.size - 10, Charsets.ISO_8859_1),
        )
    if (open.coverWidth != COVER_WIDTH || open.coverHeight != COVER_HEIGHT ||
        open.textWidth != TEXT_WIDTH || open.textHeight != TEXT_HEIGHT
    ) {
        fail(ProtocolError.IMAGE_SIZE)
    }
    if (open.link.length > MAX_LINK || !open.link.all { isLinkChar(it.code) }) fail(ProtocolError.INVALID_LINK)
    return open
}

fun encodeImagePacket(packet: ImagePacket): ByteArray = bytes(*u16(packet.counter), *u16(packet.offset)) + packet.data

fun decodeImagePacket(
    bytes: ByteArray,
    imageSize: Int,
): ImagePacket {
    check(bytes.size > PACKET_HEADER)
    val offset = bytes.u16(2)
    if (offset + bytes.size - PACKET_HEADER > imageSize) fail(ProtocolError.BEYOND_IMAGE)
    return ImagePacket(bytes.u16(0), offset, bytes.copyOfRange(PACKET_HEADER, bytes.size))
}

fun encodeState(state: PlaybackState): ByteArray = bytes(PROTOCOL_VERSION, state.code)

fun decodeState(bytes: ByteArray): PlaybackState {
    check(bytes.isNotEmpty())
    if (bytes.u8(0) != PROTOCOL_VERSION) fail(ProtocolError.UNKNOWN_VERSION)
    check(bytes.size == 2 && isState(bytes.u8(1)))
    return PlaybackState.entries.first { it.code == bytes.u8(1) }
}

fun encodeControl(message: ControlMessage): ByteArray =
    when (message) {
        is Hello -> {
            require(message.firmware.size == 3) { "the firmware version is major, minor, patch" }
            bytes(HELLO, PROTOCOL_VERSION, *u8s(*message.firmware.toIntArray()), if (message.held) 1 else 0) +
                bytes(*u32(message.identity))
        }

        is Acknowledgement -> {
            bytes(ACKNOWLEDGEMENT, *u16(message.counter))
        }

        is ResendRequest -> {
            bytes(RESEND_REQUEST, *u16(message.counter))
        }

        is Refusal -> {
            bytes(REFUSAL, requireNotNull(message.reason.reason) { "${message.reason} is never sent" })
        }
    }

fun decodeControl(bytes: ByteArray): ControlMessage {
    check(bytes.isNotEmpty())
    return when (bytes.u8(0)) {
        HELLO -> {
            check(bytes.size >= 2)
            if (bytes.u8(1) != PROTOCOL_VERSION) fail(ProtocolError.UNKNOWN_VERSION)
            check(bytes.size == 10 && bytes.u8(5) <= 1)
            val hello = Hello(listOf(bytes.u8(2), bytes.u8(3), bytes.u8(4)), bytes.u8(5) == 1, bytes.u32(6))
            check(hello.held || hello.identity == 0L)
            hello
        }

        ACKNOWLEDGEMENT, RESEND_REQUEST -> {
            check(bytes.size == 3)
            if (bytes.u8(0) == ACKNOWLEDGEMENT) Acknowledgement(bytes.u16(1)) else ResendRequest(bytes.u16(1))
        }

        REFUSAL -> {
            check(bytes.size == 2)
            Refusal(ProtocolError.entries.firstOrNull { it.reason == bytes.u8(1) } ?: fail(ProtocolError.MALFORMED))
        }

        else -> {
            fail(ProtocolError.MALFORMED)
        }
    }
}

// CRC-32/ISO-HDLC of the cover, then the text, then the link: the identity of a bundle.
fun identity(
    cover: ByteArray,
    text: ByteArray,
    link: String,
): Long =
    java.util.zip
        .CRC32()
        .apply {
            update(cover)
            update(text)
            update(link.toByteArray(Charsets.US_ASCII))
        }.value

private fun fail(error: ProtocolError): Nothing = throw ProtocolException(error)

// A malformed message, in the decoders.
private fun check(condition: Boolean) {
    if (!condition) fail(ProtocolError.MALFORMED)
}

private fun isState(code: Int) = PlaybackState.entries.any { it.code == code }

private fun isLinkChar(code: Int) = code in 0x21..0x7E // printable ASCII

private fun ByteArray.u8(index: Int) = this[index].toInt() and 0xFF

private fun ByteArray.u16(index: Int) = u8(index) or (u8(index + 1) shl 8)

private fun ByteArray.u32(index: Int) = (0..3).sumOf { u8(index + it).toLong() shl (8 * it) }

private fun bytes(vararg values: Int) = ByteArray(values.size) { values[it].toByte() }

private fun u8s(vararg values: Int): IntArray {
    require(values.all { it in 0..0xFF }) { "a value does not fit in one byte" }
    return values
}

private fun u16(value: Int): IntArray {
    require(value in 0..0xFFFF) { "a counter or an offset does not fit in two bytes" }
    return intArrayOf(value and 0xFF, value shr 8)
}

private fun u32(value: Long): IntArray {
    require(value in 0..0xFFFF_FFFFL) { "an identity does not fit in four bytes" }
    return IntArray(4) { ((value shr (8 * it)) and 0xFF).toInt() }
}
