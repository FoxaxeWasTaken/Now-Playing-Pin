#include <cstdio>

#include "screen.h"

int main() {
    std::printf("Now Playing Pin simulator, screen %dx%d\n", npp::screen_width(),
                npp::screen_height());
    return 0;
}
