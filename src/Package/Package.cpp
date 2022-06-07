/**
 *  Copyright (C) 2021 - 2022 The MRH Project Authors.
 * 
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
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
