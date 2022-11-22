// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <__msvc_int128.hpp>

typedef std::_Unsigned128 __uint128_t;
typedef std::_Signed128 __int128_t;

#include <map>
#include <string>
#include <cstdlib>
#include <array>
#include <algorithm>

namespace fmt {
	inline namespace v9 {

		using int128_opt = __int128_t;  // An optional native 128-bit integer.
		using uint128_opt = __uint128_t;

		namespace detail {
			template <typename T> inline auto convert_for_visit(T value) -> T {
				return value;
			}
		}
	}
}

#define FMT_USE_INT128 1
#include <fmt/core.h>

#define USE_INTERVAL_TREE_NAMESPACE
#include <IntervalTree.h>

#endif //PCH_H
