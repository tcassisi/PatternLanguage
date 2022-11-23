#pragma once

#include <cstddef>
#include <cstdint>

//#ifdef _MSC_VER
//#ifndef __MSVC_INT128_HPP
//#include <ranges>
//typedef std::_Unsigned128 __uint128_t;
//typedef std::_Signed128 __int128_t;
//#endif
//#endif

namespace pl {

    using u8   = std::uint8_t;
    using u16  = std::uint16_t;
    using u32  = std::uint32_t;
    using u64  = std::uint64_t;
#ifdef _MSC_VER
    using u128 = u64;
#else
    using u128 = __uint128_t;
#endif

    using i8   = std::int8_t;
    using i16  = std::int16_t;
    using i32  = std::int32_t;
    using i64  = std::int64_t;
#ifdef _MSC_VER
    using i128 = i64;
#else
    using i128 = __int128_t;
#endif

    struct Region {
        u64 address;
        size_t size;
    };

}