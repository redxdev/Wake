#pragma once

#include <memory>

namespace wake
{
    // Standard RC shared pointer type.
    // This is typedef'd as it may be replaced with a custom implementation in the future.
    template<typename T>
    using SharedPtr = std::shared_ptr<T>;
}