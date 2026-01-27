/**
 * @file main.cpp
 * @author Luke Houston (Romket) (lukehouston08@gmail.com)
 * @brief main file and entry point for ccase-check
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

int main(int argc, char** argv)
{
    auto parsedArgs = Parser::CommandLine(argc, argv);

    if (!parsedArgs)
    {
        Error& err = parsedArgs.error();
        switch (err.Type)
        {
            case Error::ErrType::noInput:
                std::cout << "Error: no input files specified" << std::endl;
                break;
            case Error::ErrType::scanPathDNE:
                std::cout << "Error: scan path specified does not exist:"
                          << err.Info << std::endl;
                break;
            case Error::ErrType::configPathDNE:
                std::cout << "Error: config path specified does not exist:"
                          << err.Info << std::endl;
                break;
            case Error::ErrType::multipleConfigs:
                std::cout << "Error: multiple config paths specified: "
                          << err.Info << std::endl;
                break;
            case Error::ErrType::unknownOption:
                std::cout << "Error: unknown option: " << err.Info << std::endl;
                break;
            case Error::ErrType::extraOptions:
                std::cout << "Error: too many arguments given" << std::endl;
                break;
            case Error::ErrType::dontScan: return 0;
        }

        Parser::DisplayArgs();

        return 1;
    }
}
