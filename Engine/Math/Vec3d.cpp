#include "Vec3d.h"

// Vec3d and Quatd are header-only; this translation unit ensures the
// library has at least one compiled symbol for the linker.

namespace ednms {
namespace detail {
    // Anchor symbol so the static library is never empty.
    void math_anchor() {}
} // namespace detail
} // namespace ednms
