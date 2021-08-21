/**
 *  PackageService.cpp
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

// External

// Project
#include "./PackageService.h"


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************

PackageService::PackageService(std::string const& s_ServiceBinaryPath,
                               std::string const& s_ConfigurationPath) : PackageConfiguration(s_ConfigurationPath)
{
    this->s_ServiceBinaryPath = s_ServiceBinaryPath;
}

PackageService::PackageService(PackageService const& c_PackageService) noexcept : PackageConfiguration(c_PackageService),
                                                                                  s_ServiceBinaryPath(c_PackageService.s_ServiceBinaryPath)
{}

PackageService::~PackageService() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

std::string PackageService::GetServiceBinaryPath() const noexcept
{
    return s_ServiceBinaryPath;
}
