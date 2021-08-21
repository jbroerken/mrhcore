/**
 *  PackageService.h
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

#ifndef PackageService_h
#define PackageService_h

// C / C++

// External

// Project
#include "./PackageConfiguration.h"


class PackageService : public PackageConfiguration
{
public:

    //*************************************************************************************
    // Getters
    //*************************************************************************************
     
    /**
     *  Get the package service binary path.
     *
     *  \return The full path to the package service binary.
     */

    std::string GetServiceBinaryPath() const noexcept;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    std::string s_ServiceBinaryPath;

protected:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_ServiceBinaryPath The full path to the package service binary.
     *  \param s_ConfigurationPath The full path to the package configuration file.
     */

    PackageService(std::string const& s_ServiceBinaryPath,
                   std::string const& s_ConfigurationPath);

    /**
     *  Copy constructor.
     *
     *  \param c_PackageService PackageService class source.
     */
    
    PackageService(PackageService const& c_PackageService) noexcept;
    
    /**
     *  Default destructor.
     */

    ~PackageService() noexcept;
};

#endif /* PackageService_h */
