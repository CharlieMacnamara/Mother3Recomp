#include "m3_intro.h"

#include "input_synth.h"
#include "runtime_arm.h"

namespace {

// gEngineMode._0 and gSomeBlend + 0x2CE4 (see m3_intro.h).
constexpr std::uint32_t kEngineModeAddress = 0x02004848;
constexpr std::uint32_t kTitleSceneAddress = 0x02018D0C;
// A boot that never reaches the title is a bug, not a reason to keep
// pressing buttons: give up after 90 seconds.
constexpr std::uint64_t kGiveUpFrames = 90 * 60;

gbarecomp::KeySynth g_synth;
bool g_enabled = false;
bool g_finished = false;
std::uint64_t g_frames = 0;

}  // namespace

void m3intro::enable(bool on) {
    g_enabled = on;
    g_finished = !on;
    g_frames = 0;
    g_synth.clear();
}

std::uint16_t m3intro::input_frame(const gbarecomp::TouchFrameInfo*) {
    if (!g_enabled || g_finished) return 0x03FFu;
    if (++g_frames > kGiveUpFrames) {
        g_finished = true;
        g_synth.clear();
        return 0x03FFu;
    }

    const auto engine = static_cast<std::uint8_t>(bus_read_u8(kEngineModeAddress));
    const auto scene = static_cast<std::uint8_t>(bus_read_u8(kTitleSceneAddress));
    if (title_reached(engine, scene)) {
        g_finished = true;
        g_synth.clear();
        return 0x03FFu;
    }

    // One short press at a time. A new press needs a released frame first,
    // which KeySynth enforces, so the splash screens see a clean edge.
    if (g_synth.idle()) g_synth.tap(gbarecomp::kGbaKeyA, 1, 11);
    return g_synth.frame();
}

void m3intro::savestate_loaded(const char*) {
    g_enabled = false;
    g_finished = true;
    g_synth.clear();
}
