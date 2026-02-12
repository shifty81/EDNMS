#pragma once
#include <cstdint>

namespace ednms {

using EntityID = uint64_t;
static constexpr EntityID INVALID_ENTITY = 0;

using ResourceID = uint32_t;
using BlueprintID = uint32_t;
using FactionID = uint32_t;

} // namespace ednms
