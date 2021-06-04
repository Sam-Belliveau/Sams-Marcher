#ifndef SPGL_TYPENAMES_HPP
#define SPGL_TYPENAMES_HPP 1

#include <cstdint>

namespace SPGL
{
    /* Floats */
    using Float80 = long double;
    using Float64 = double;
    using Float32 = float;

    using FloatMax = Float80;

    /* Integers */
    using Size = std::size_t;

    using UIntMax = std::uintmax_t;
    using IntMax = std::intmax_t;

    using UInt64 = std::uint64_t;
    using Int64 = std::int64_t;

    using UInt32 = std::uint32_t;
    using Int32 = std::int32_t;

    using UInt16 = std::uint16_t;
    using Int16 = std::int16_t;

    using UInt8 = std::uint8_t;
    using Int8 = std::int8_t;
}

#endif //SPGL_TYPENAMES_HPP
