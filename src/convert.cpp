/**
 * @file cases.cpp
 * @author Luke Houston (Romket) (lukehouston08@gmail.com)
 * @brief
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

#include <convert.h>

std::regex Convert::CaseNameToRegex(std::string_view typeString)
{
    auto it = _caseNameToPattern.find(typeString);
    if (it != _caseNameToPattern.end()) return it->second;

    return std::regex {std::move(std::string {typeString})};
}

Contexts Convert::StrToContext(std::string_view contextString)
{
    auto it = _strToContext.find(contextString);
    if (it != _strToContext.end()) return it->second;

    throw std::invalid_argument {"Invalid option \"" +
                                 std::string {contextString} + "\"."};
}
