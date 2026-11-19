#include "screen.h"

#include <doctest/doctest.h>

TEST_CASE("the screen is the 2.13 inch panel in landscape") {
    CHECK(npp::screen_width() == 250);
    CHECK(npp::screen_height() == 122);
}
