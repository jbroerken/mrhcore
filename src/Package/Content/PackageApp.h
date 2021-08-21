/**
 *  PackageApp.h
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

#ifndef PackageApp_h
#define PackageApp_h

// C / C++

// External

// Project
#include "./PackageConfiguration.h"


class PackageApp : public PackageConfiguration
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_AppBinaryPath The full path to the package service binary.
     *  \param s_ConfigurationPath The full path to the package configuration file.
     */

    PackageApp(std::string const& s_AppBinaryPath,
               std::string const& s_ConfigurationPath);

    /**
     *  Copy constructor.
     *
     *  \param c_PackageApp PackageApp class source.
     */
    
    PackageApp(PackageApp const& c_PackageApp) noexcept;
    
    /**
     *  Default destructor.
     */

    ~PackageApp() noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************
      
     /**
      *  Get the package app binary path.
      *
      *  \return The full path to the package app binary.
      */

     std::string GetAppBinaryPath() const noexcept;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::string s_AppBinaryPath;

protected:

};

#endif /* PackageApp_h */
