/**
 * @file scanner.h
 * @author Luke Houston (Romket) (lukehouston08@gmail.com)
 * @brief definition for Scanner class
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

#include <filesystem>
#include <vector>

struct ScanInfo
{
    std::filesystem::path              ConfigPath = ".ccase-check";
    std::vector<std::filesystem::path> ToScan;
};

class Scanner
{
public:
    Scanner(const ScanInfo&& info);

    bool StartScan();

private:
    bool loadConfig();

    std::filesystem::path              _configPath;
    std::vector<std::filesystem::path> _toScan;
};
