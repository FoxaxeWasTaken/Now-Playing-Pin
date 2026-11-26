package io.github.foxaxewastaken.nowplayingpin.protocol

import org.junit.Assert.assertArrayEquals
import org.junit.Assert.assertEquals
import org.junit.Assert.assertNotEquals
import org.junit.Assert.assertThrows
import org.junit.Assert.assertTrue
import org.junit.Test
import java.io.File

// Contract tests: every vector of tools/vectors/ in both directions (docs/testing.md,
// "Level 2: contract tests"). The same files are run by firmware/tests/protocol_test.cpp.
class ProtocolTest {
    @Test
    fun `Track vectors, both directions`() =
        run("track.txt", { _, bytes -> decodeTrack(bytes) }) { v, bytes ->
            val expected =
                when (v.getValue("message")) {
                    "open" -> {
                        Open(
                            counter = int(v, "counter"),
                            state = PlaybackState.valueOf(v.getValue("state").uppercase()),
                            coverWidth = int(v, "cover_width"),
                            coverHeight = int(v, "cover_height"),
                            textWidth = int(v, "text_width"),
                            textHeight = int(v, "text_height"),
                            link = v.getValue("link"),
                        )
                    }

                    "end" -> {
                        End(counter = int(v, "counter"), identity = number(v.getValue("identity")))
                    }

                    else -> {
                        error("unknown message ${v["message"]}")
                    }
                }
            assertEquals(expected, decodeTrack(bytes))
            assertArrayEquals(bytes, encodeTrack(expected))
        }

    @Test
    fun `image packet vectors, both directions`() =
        run("image.txt", { v, bytes -> decodeImagePacket(bytes, int(v, "image_size")) }) { v, bytes ->
            assertEquals("packet", v.getValue("message"))
            val data = hex(v.getValue("data"))
            val decoded = decodeImagePacket(bytes, int(v, "image_size"))
            assertEquals(int(v, "counter"), decoded.counter)
            assertEquals(int(v, "offset"), decoded.offset)
            assertArrayEquals(data, decoded.data)
            assertArrayEquals(bytes, encodeImagePacket(ImagePacket(int(v, "counter"), int(v, "offset"), data)))
        }

    @Test
    fun `State vectors, both directions`() =
        run("state.txt", { _, bytes -> decodeState(bytes) }) { v, bytes ->
            assertEquals("state", v.getValue("message"))
            val expected = PlaybackState.valueOf(v.getValue("state").uppercase())
            assertEquals(expected, decodeState(bytes))
            assertArrayEquals(bytes, encodeState(expected))
        }

    @Test
    fun `Control vectors, both directions`() =
        run("control.txt", { _, bytes -> decodeControl(bytes) }) { v, bytes ->
            val expected =
                when (v.getValue("message")) {
                    "hello" -> {
                        Hello(
                            firmware = v.getValue("firmware").split('.').map { it.toInt() },
                            held = int(v, "held") == 1,
                            identity = number(v.getValue("identity")),
                        )
                    }

                    "acknowledgement" -> {
                        Acknowledgement(int(v, "counter"))
                    }

                    "resend_request" -> {
                        ResendRequest(int(v, "counter"))
                    }

                    "refusal" -> {
                        Refusal(ProtocolError.valueOf(v.getValue("reason").uppercase()))
                    }

                    else -> {
                        error("unknown message ${v["message"]}")
                    }
                }
            assertEquals(expected, decodeControl(bytes))
            assertArrayEquals(bytes, encodeControl(expected))
        }

    @Test
    fun `identity vectors, the CRC-32 of cover, text and link`() {
        val vectors = load("identity.txt")
        assertTrue(vectors.isNotEmpty())
        for (v in vectors) {
            val name = v.getValue("name")
            val computed = identity(hex(v.getValue("cover")), hex(v.getValue("text")), v.getValue("link"))
            val given = number(v.getValue("identity"))
            when (v["error"]) {
                null -> assertEquals(name, given, computed)
                "wrong_crc" -> assertNotEquals(name, given, computed)
                else -> error("unknown error ${v["error"]} in \"$name\"")
            }
        }
    }

    // Runs `valid` on each valid vector, and checks that `decode` refuses each invalid one with
    // its expected error.
    private fun run(
        file: String,
        decode: (Map<String, String>, ByteArray) -> Any,
        valid: (Map<String, String>, ByteArray) -> Unit,
    ) {
        val vectors = load(file)
        assertTrue(vectors.isNotEmpty())
        for (v in vectors) {
            val name = v.getValue("name")
            val bytes = hex(v.getValue("bytes"))
            try {
                val error = v["error"]
                if (error == null) {
                    v["version"]?.let { assertEquals(PROTOCOL_VERSION, it.toInt()) }
                    valid(v, bytes)
                } else {
                    val thrown = assertThrows(ProtocolException::class.java) { decode(v, bytes) }
                    assertEquals(error, thrown.error.name.lowercase())
                }
            } catch (e: Throwable) {
                throw AssertionError("vector \"$name\" of $file: ${e.message}", e)
            }
        }
    }

    // Reads a vector file: blocks of "key: value" lines, separated by blank lines.
    private fun load(file: String): List<Map<String, String>> {
        val dir = System.getProperty("vectors.dir") ?: error("vectors.dir is not set (app/build.gradle.kts)")
        val vectors = mutableListOf<MutableMap<String, String>>(mutableMapOf())
        for (line in File(dir, file).readLines()) {
            when {
                line.isEmpty() -> {
                    if (vectors.last().isNotEmpty()) vectors.add(mutableMapOf())
                }

                line.startsWith("#") -> {
                    Unit
                }

                else -> {
                    val colon = line.indexOf(':')
                    require(colon > 0) { "not a key: value line: $line" }
                    vectors.last()[line.substring(0, colon)] = line.substring(colon + 1).trim()
                }
            }
        }
        return vectors.filter { it.isNotEmpty() }
    }

    private fun hex(text: String): ByteArray =
        text
            .split(' ')
            .filter { it.isNotEmpty() }
            .map { it.toInt(16).toByte() }
            .toByteArray()

    private fun number(text: String): Long = if (text.startsWith("0x")) text.substring(2).toLong(16) else text.toLong()

    private fun int(
        v: Map<String, String>,
        key: String,
    ): Int = number(v.getValue(key)).toInt()
}
