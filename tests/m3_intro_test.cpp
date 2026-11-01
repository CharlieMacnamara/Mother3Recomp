// The intro skip only stops on the title scene's menu state.
#include <cstdio>

#include "m3_intro.h"

namespace {

int failures = 0;

void check(bool condition, const char* what, int line) {
    if (condition) return;
    std::fprintf(stderr, "m3_intro_test:%d: %s\n", line, what);
    ++failures;
}

#define CHECK(cond) check((cond), #cond, __LINE__)

}  // namespace

int main() {
    // The title scene reaches 0x0C just before the menu and 0x0D at it.
    CHECK(m3intro::title_reached(0x0A, 0x0C));
    CHECK(m3intro::title_reached(0x0A, 0x0D));

    // Splash screens run in other engine modes; their scene byte is not a
    // reason to stop tapping.
    CHECK(!m3intro::title_reached(0x0E, 0x0C));
    CHECK(!m3intro::title_reached(0x0D, 0x0C));

    // Before the title scene initialises the byte can hold anything; only
    // the documented range counts.
    CHECK(!m3intro::title_reached(0x0A, 0x0B));
    CHECK(!m3intro::title_reached(0x0A, 0x78));
    CHECK(!m3intro::title_reached(0x0A, 0x14));

    return failures == 0 ? 0 : 1;
}
