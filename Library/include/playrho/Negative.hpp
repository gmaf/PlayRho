/*
 * Copyright (c) 2023 Louis Langholtz https://github.com/louis-langholtz/PlayRho
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef PLAYRHO_NEGATIVE_HPP
#define PLAYRHO_NEGATIVE_HPP

#include <playrho/CheckedValue.hpp>

namespace playrho {

/// @brief Negative constrained value checker.
template <typename T>
struct NegativeChecker {

    /// @brief Value checking functor.
    constexpr auto operator()(const T& v) noexcept
        -> decltype(v < static_cast<T>(0), static_cast<const char*>(nullptr))
    {
        if (!(v < static_cast<T>(0))) {
            return "value not less than zero";
        }
        return {};
    }
};

/// @ingroup CheckedValues
/// @brief Negative constrained value type.
template <typename T>
using Negative = CheckedValue<T, NegativeChecker<T>>;

static_assert(!std::is_default_constructible<Negative<int>>::value);

} // namespace playrho

#endif // PLAYRHO_NEGATIVE_HPP
