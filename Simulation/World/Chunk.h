#pragma once
#include <cstdint>

namespace ednms {

struct ChunkCoord {
    int32_t x = 0;
    int32_t y = 0;
    int32_t z = 0;
};

enum class ChunkSimState {
    Full,
    LowFidelity,
    Unloaded
};

struct ChunkRuntime {
    uint64_t id = 0;
    ChunkCoord coord;
    ChunkSimState state = ChunkSimState::Unloaded;
};

} // namespace ednms
