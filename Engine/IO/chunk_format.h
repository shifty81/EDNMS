#pragma once
#include <cstdint>

namespace ednms {

static constexpr uint32_t CHUNK_MAGIC = 0x43484B31; // "CHK1"

struct ChunkHeader {
    uint32_t magic = 0;
    uint32_t version = 0;
    uint64_t chunkID = 0;
    uint32_t entityCount = 0;
    uint32_t componentCount = 0;
};

enum class ChunkState {
    Unloaded,
    LowFidelitySim,
    FullSim
};

} // namespace ednms
