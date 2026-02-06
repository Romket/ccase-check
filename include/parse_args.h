/**
 * @file parse_args.h
 * @author Luke Houston (Romket) (lukehouston08@gmail.com)
 * @brief contains functions to parse input
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

#include <scanner.h>

#include <expected>
#include <optional>
#include <string>

enum PathType
{
    code,
    configFile
};

struct Error
{
    enum class ErrType
    {
        dontScan          = 0,
        noInput           = 2,
        scanPathDNE       = 3,
        configPathDNE     = 4,
        ignorePathDNE     = 5,
        configPathNotFile = 6,
        ignorePathNotFile = 7,
        multipleConfigs   = 8,
        multipleIgnores   = 9,
        unknownOption     = 10,
        extraOptions      = 11
    };

    ErrType     Type;
    std::string Info {};
};

struct OptionInfo
{
    std::string_view Option;
    std::string_view Arg;
    int              Argc;
    ScanInfo&        Scan;
    bool             ConfigPathSpecified;
    bool             IgnorePathSpecified;
};

class Parser
{
public:
    static const std::expected<ScanInfo, Error> CommandLine(int    argc,
                                                            char** argv);

    static int DisplayErrors(const Error& err);

private:
    static const std::optional<Error> handleOptions(const OptionInfo&& info);

    static const std::optional<Error>
    handleCodePath(const std::string_view path, ScanInfo& info);

    static void displayArgs();
};
