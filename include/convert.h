/**
 * @file cases.h
 * @author Luke Houston (Romket) (lukehouston08@gmail.com)
 * @brief all cases, string mappings, and regex
 * @version 0.1
 * @date 2026-01-16
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

#include <contexts.h>
#include <heterogeneous_lookup.h>

#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>

class Convert
{
public:
    static std::regex CaseNameToRegex(std::string_view typeString);

    static Contexts StrToContext(std::string_view contextString);

private:
    inline static const std::unordered_map<std::string, std::regex, stringHash,
                                           std::equal_to<>>
        _caseNameToPattern {
            {"CamelCase", std::regex("[a-z][A-Za-z0-9]*")},
            {"PascalCase", std::regex("[A-Z][A-Za-z0-9]*")},
            {"SnakeCase", std::regex("[a-z](?:_|[a-z0-9])*")},
            {"ScreamingSnakeCase", std::regex("[A-Z](?:_|[A-Z0-9])*")},
            {"KebabCase", std::regex("[a-z](?:-|[a-z0-9])*")},
            {"TrainCase", std::regex("[A-Z](?:-[A-Z]|[a-z0-9])*")},
            {"FlatCase", std::regex("[a-z0-9]*")}};

    inline static const std::unordered_map<std::string, Contexts, stringHash,
                                           std::equal_to<>>
        _strToContext {{"classDef", Contexts::cClass},
                       {"structDef", Contexts::cStruct},
                       {"enumDef", Contexts::cEnum},

                       {"globalFunc", Contexts::cFunction},
                       {"globalVar", Contexts::cVariable},

                       {"publicFunc", Contexts::cPublicFunction},
                       {"publicVar", Contexts::cPublicVariable},
                       {"protectedFunc", Contexts::cProtectedFunction},
                       {"protectedVar", Contexts::cProtectedVariable},
                       {"privateFunc", Contexts::cPrivateFunction},
                       {"privateVar", Contexts::cPrivateVariable}};
};
