/**
 * @file heterogeneous_lookup.h
 * @author Luke Houston (Romket) (lukehouston08@gmail.com)
 * @brief implements heterogeneous lookup for std::unordered_map
 * @version 0.1
 * @date 2026-01-20
 *
 * @copyright Copyright (c) 2026 Luke Houston
 *
 * This file is part of ccase-check.  ccase-check is free software:
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as publishedby the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#pragma once

#include <string>
#include <string_view>

template<typename... Bases>
struct Overload : Bases...
{
    using is_transparent = void;
    using Bases::operator()...;
};

struct CharPtrHash
{
    auto operator()(const char* ptr) const noexcept
    {
        return std::hash<std::string_view> {}(ptr);
    }
};

using stringHash =
    Overload<std::hash<std::string>, std::hash<std::string_view>, CharPtrHash>;
