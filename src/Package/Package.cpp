/**
 *  Package.cpp
 *
 *  This file is part of the MRH project.
 *  See the AUTHORS file for Copyright information.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

// C / C++
#include <fstream>
#include <cerrno>
#include <cstring>

// External

// Project
#include "./Package.h"
#include "./PackagePaths.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

Package::Package(std::string const& s_DirectoryPath,
                 std::string const& s_PackageName) : PackageApp(s_DirectoryPath + s_PackageName + "/" + PACKAGE_APP_BINARY_PATH,
                                                                s_DirectoryPath + s_PackageName + "/" + PACKAGE_CONFIGURATION_PATH),
                                                     PackageService(s_DirectoryPath + s_PackageName + "/" + PACKAGE_SERVICE_BINARY_PATH,
                                                                    s_DirectoryPath + s_PackageName + "/" + PACKAGE_CONFIGURATION_PATH)
{
    if ((s_PackagePath = s_DirectoryPath).length() == 0)
    {
        s_PackagePath = "/";
    }
    else if (*(s_PackagePath.end() - 1) != '/')
    {
        s_PackagePath += "/";
    }
    
    this->s_PackagePath += (s_PackageName + "/");
    this->s_PackageName = s_PackageName;
}

Package::Package(Package const& c_Package) noexcept : PackageApp(c_Package),
                                                      PackageService(c_Package),
                                                      s_PackagePath(c_Package.s_PackagePath),
                                                      s_PackageName(c_Package.s_PackageName)
{}

Package::~Package() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string Package::GetPackagePath() const noexcept
{
    return s_PackagePath;
}

std::string Package::GetPackageName() const noexcept
{
    return s_PackageName;
}
