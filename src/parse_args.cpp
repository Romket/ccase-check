/**
 * @file parse_args.cpp
 * @author Luke Houston (Romket) (lukehouston08@gmail.com)
 * @brief implementation of Parser class
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

#include <parse_args.h>

#include <iostream>
#include <utility>

const std::expected<ScanInfo, Error> Parser::CommandLine(int argc, char** argv)
{
    if (argc == 1) return std::unexpected {Error {Error::ErrType::noInput}};

    ScanInfo info;
    bool     configSpecified {false};
    for (int i = 1; i < argc; ++i)
    {
        if (std::string_view {argv[i]}.length() < 3)
        {
            auto err {handleCodePath(argv[i], info)};
            if (err) return std::unexpected {*err};
            continue;
        }

        std::string_view isOption {std::string_view(argv[i]).substr(0, 2)};
        std::string_view option {std::string_view(argv[i]).substr(2)};

        if (isOption == "--")
        {
            auto err {
                handleOptions({option, argv[i], argc, info, configSpecified})};
            if (err) return std::unexpected {*err};
            continue;
        }

        auto err {handleCodePath(argv[i], info)};
        if (err) return std::unexpected {*err};
    }

    if (!std::filesystem::exists(info.ConfigPath))
    {
        return std::unexpected {
            Error {Error::ErrType::configPathDNE, info.ConfigPath}};
    }

    if (!std::filesystem::is_regular_file(info.ConfigPath))
    {
        return std::unexpected {
            Error {Error::ErrType::configPathNotFile, info.ConfigPath}};
    }

    return info;
}

int Parser::DisplayErrors(const Error& err)
{
    switch (err.Type)
    {
        case Error::ErrType::noInput:
            std::cout << "Error: no input files specified\n";
            break;
        case Error::ErrType::scanPathDNE:
            std::cout << "Error: scan path specified does not exist: "
                      << err.Info << '\n';
            break;
        case Error::ErrType::configPathDNE:
            std::cout << "Error: config path specified does not exist: "
                      << err.Info << '\n';
            break;
        case Error::ErrType::ignorePathDNE:
            std::cout << "Error: ignore file specified does not exist: "
                      << err.Info << '\n';
        case Error::ErrType::configPathNotFile:
            std::cout << "Error: config path specified is not a file: "
                      << err.Info << '\n';
            break;
        case Error::ErrType::ignorePathNotFile:
            std::cout << "Error: ignore file specified is not a file: "
                      << err.Info << '\n';
            break;
        case Error::ErrType::multipleConfigs:
            std::cout << "Error: multiple config paths specified: " << err.Info
                      << '\n';
            break;
        case Error::ErrType::multipleIgnores:
            std::cout << "Error: multiple ignore paths specified: " << err.Info
                      << '\n';
            break;
        case Error::ErrType::unknownOption:
            std::cout << "Error: unknown option: " << err.Info << '\n';
            break;
        case Error::ErrType::extraOptions:
            std::cout << "Error: too many arguments given\n";
            break;
        case Error::ErrType::dontScan: return 0;
    }

    displayArgs();
    return std::to_underlying(err.Type);
}

const std::optional<Error> Parser::handleOptions(const OptionInfo&& info)
{
    if (info.Option.substr(0, 7) == "config=")
    {
        if (info.ConfigPathSpecified)
        {
            return Error {Error::ErrType::multipleConfigs,
                          std::string {info.Scan.ConfigPath} + " and " +
                              std::string {info.Arg.substr(9)}};
        }

        info.Scan.ConfigPath = std::move(info.Arg.substr(9));

        if (!std::filesystem::exists(info.Scan.ConfigPath))
        {
            return Error {Error::ErrType::configPathDNE, info.Scan.ConfigPath};
        }

        if (!std::filesystem::is_regular_file(info.Scan.ConfigPath))
        {
            return Error {Error::ErrType::configPathNotFile,
                          info.Scan.ConfigPath};
        }
    }
    else if (info.Option.substr(0, 7) == "ignore=")
    {
        if (info.IgnorePathSpecified)
        {
            return Error {Error::ErrType::multipleIgnores,
                          std::string {info.Scan.IgnorePath} + " and " +
                              std::string {info.Arg.substr(9)}};
        }

        info.Scan.IgnorePath = std::move(info.Arg.substr(9));

        if (!std::filesystem::exists(info.Scan.IgnorePath))
        {
            return Error {Error::ErrType::ignorePathDNE, info.Scan.IgnorePath};
        }

        if (!std::filesystem::is_regular_file(info.Scan.IgnorePath))
        {
            return Error {Error::ErrType::ignorePathNotFile,
                          info.Scan.IgnorePath};
        }
    }
    else if (info.Option.substr(0, 4) == "help")
    {
        if (info.Argc != 2) return Error {Error::ErrType::extraOptions};
        displayArgs();
        return Error {Error::ErrType::dontScan};
    }
    else if (info.Option.substr(0, 7) == "version")
    {
        if (info.Argc != 2) return Error {Error::ErrType::extraOptions};
        std::cout << "ccase-check version " << MAJOR_VERSION << '.'
                  << MINOR_VERSION << '.' << PATCH_VERSION << std::endl;
        return Error {Error::ErrType::dontScan};
    }
    else
    {
        return Error {Error::ErrType::unknownOption, std::string(info.Arg)};
    }

    return std::nullopt;
}

void Parser::displayArgs()
{
    std::cout << "\
Usage: ccase-check [options] <input files>\n\n\
Options:\n\n\
Generic Options:\n\n\
  --help                        - Display this message\n\
  --version                     - Display the version of this program\n\n\
ccase-check options:\n\n\
  --config=<config path>        - Override the default config path If not\n\
                                  specified, the program will look for a\n\
                                  .ccase-check file in the current directory."
              << std::endl;
}

const std::optional<Error> Parser::handleCodePath(std::string_view path,
                                                  ScanInfo&        info)
{
    std::filesystem::path codePath {std::move(path)};

    if (!std::filesystem::exists(codePath))
    {
        return Error {Error::ErrType::scanPathDNE, codePath};
    }

    info.ToScan.push_back(std::move(codePath));

    return std::nullopt;
}
