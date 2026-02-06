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
#include <iostream>

Scanner::Scanner(const ScanInfo&& info)
{
    _configPath = std::move(info.ConfigPath);
    _ignorePath = std::move(info.IgnorePath);
    _toScan     = std::move(info.ToScan);
}

int Scanner::Run()
{
    if (int err = loadConfig() != 0) return err;

    for (const auto& path : _toScan)
    {
        if (std::filesystem::is_directory(path))
        {
            if (!scanDir(std::move(path))) return 1;
        }
        else if (std::filesystem::is_regular_file(path))
        {
            if (!scanFile(std::move(path))) return 1;
        }
        else
        {
            std::cout << "Invalid file found: " << path << '\n';
            return -2;
        }
    }

    return 0;
}

int Scanner::loadConfig()
{
    std::ifstream file {_configPath};
    if (!file)
    {
        std::cout << "Failed to load file\n";
        return -1;
    }

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
                std::cout << "Duplicate argument " << node.key() << '\n';
                return -10;
            }

            _patternMap.emplace(std::pair {
                std::move(c),
                Convert::CaseNameToRegex(std::string_view {node.val()})});
        }
        catch (const std::exception& e)
        {
            std::cout << e.what() << '\n';
            return -10;
        }
    }

    return 0;
}

void Scanner::loadIgnore()
{
    if (!std::filesystem::exists(_ignorePath) ||
        !std::filesystem::is_regular_file(_ignorePath))
        return;
}

bool Scanner::scanDir(const std::filesystem::path&& dir)
{
    for (const auto& entry :
         std::filesystem::directory_iterator {std::move(dir)})
    {
        if (entry.is_directory())
        {
            if (!scanDir(std::move(entry.path()))) return false;
        }
        else if (entry.is_regular_file())
        {
            if (!scanFile(std::move(entry.path()))) return false;
        }
        else
        {
            std::cout << "Invalid file found: " << entry << '\n';
            return false;
        }
    }

    return true;
}

bool Scanner::scanFile(const std::filesystem::path&& file)
{
    // placeholder
    std::cout << "Scanning " << file << '\n';

    return true;
}
