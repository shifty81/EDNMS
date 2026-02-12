#pragma once
#include <bitset>
#include <cstddef>

namespace ednms {

static constexpr size_t MAX_COMPONENTS = 64;
using ComponentMask = std::bitset<MAX_COMPONENTS>;

} // namespace ednms
