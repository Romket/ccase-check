/**
 * @file scanner.cpp
 * @author Luke Houston (Romket) (lukehouston08@gmail.com)
 * @brief Scanner class implementation
 * @version 0.1
 * @date 2026-01-27
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

#include <scanner.h>

#include <convert.h>

#include <c4/std/string.hpp>
#include <ryml.hpp>

#include <fstream>

Scanner::Scanner(const ScanInfo&& info)
{
    _configPath = std::move(info.ConfigPath);
    _toScan     = std::move(info.ToScan);
}

std::optional<std::string> Scanner::StartScan()
{
    auto err = loadConfig();
    if (err) return err;

    return std::nullopt;
}

std::optional<std::string> Scanner::loadConfig()
{
    std::ifstream file {_configPath};
    if (!file) return "Failed to load file";

    std::string configText {std::istreambuf_iterator<char>(file),
                            std::istreambuf_iterator<char>()};

    ryml::Tree confTree {ryml::parse_in_place(ryml::to_substr(configText))};

    for (const ryml::ConstNodeRef&& node : confTree.rootref())
    {
        try
        {
            Contexts c = Convert::StrToContext(std::string_view {node.key()});
            if (_patternMap.find(c) != _patternMap.end())
            {
                return "Duplicate argument " + std::string {node.key().str};
            }

            _patternMap.emplace(std::pair {
                std::move(c),
                Convert::CaseNameToRegex(std::string_view {node.val()})});
        }
        catch (const std::exception& e)
        {
            return e.what();
        }
    }

    return std::nullopt;
}
