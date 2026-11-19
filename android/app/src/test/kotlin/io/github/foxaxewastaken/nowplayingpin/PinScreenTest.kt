package io.github.foxaxewastaken.nowplayingpin

import org.junit.Assert.assertEquals
import org.junit.Test

class PinScreenTest {
    @Test
    fun `the screen is the panel in landscape`() {
        assertEquals(250, PinScreen.WIDTH)
        assertEquals(122, PinScreen.HEIGHT)
    }
}
