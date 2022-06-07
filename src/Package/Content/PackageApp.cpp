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

// External

// Project
#include "./PackageApp.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PackageApp::PackageApp(std::string const& s_AppBinaryPath,
                       std::string const& s_ConfigurationPath) : PackageConfiguration(s_ConfigurationPath)
{
    this->s_AppBinaryPath = s_AppBinaryPath;
}

PackageApp::PackageApp(PackageApp const& c_PackageApp) noexcept : PackageConfiguration(c_PackageApp),
                                                                  s_AppBinaryPath(c_PackageApp.s_AppBinaryPath)
{}

PackageApp::~PackageApp() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string PackageApp::GetAppBinaryPath() const noexcept
{
    return s_AppBinaryPath;
}
