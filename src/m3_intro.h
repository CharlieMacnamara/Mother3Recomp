// m3_intro.h — skip MOTHER 3's startup screens and title flyover.
//
// The game shows a health-and-safety page, the fan-translation card and
// other startup screens before the title menu, then runs a flyover behind
// the logo. They all advance on A. This policy taps A until the title scene
// reaches its menu state, then stops; it never touches gameplay.
#pragma once
#include <cstdint>

namespace gbarecomp {
struct TouchFrameInfo;
}

namespace m3intro {

// gEngineMode._0 and gSomeBlend + 0x2CE4 (the title scene's state index) in
// the game's RAM layout. The title scene runs in engine mode 0x0A; its state
// machine reaches 0x0C just before the menu and 0x0D at the menu.
constexpr std::uint8_t kTitleEngine = 0x0A;
constexpr std::uint8_t kTitleMenuScene = 0x0C;
constexpr std::uint8_t kTitleLastScene = 0x13;

// True once the menu (or the transition into it) is up. The upper bound
// rejects unrelated bytes while the scene is not initialised.
inline bool title_reached(std::uint8_t engine, std::uint8_t scene) {
    return engine == kTitleEngine && scene >= kTitleMenuScene && scene <= kTitleLastScene;
}

// Enable for a fresh boot. A loaded savestate disables the policy.
void enable(bool on);

// RunOptions::input_frame. Returns active-low KEYINPUT (0x03FF = nothing).
std::uint16_t input_frame(const gbarecomp::TouchFrameInfo* frame);

// RunOptions::savestate_loaded: never tap into a resumed machine.
void savestate_loaded(const char* path);

}  // namespace m3intro
