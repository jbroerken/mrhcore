/**
 *  Package.h
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

#ifndef Package_h
#define Package_h

// C / C++

// External

// Project
#include "./Content/PackageApp.h"
#include "./Content/PackageService.h"


class Package : public virtual PackageApp,
                public virtual PackageService
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_DirectoryPath The full path to the package location directory.
     *  \param s_PackageName The full package directory name.
     */

    Package(std::string const& s_DirectoryPath,
            std::string const& s_PackageName);

    /**
     *  Copy constructor.
     *
     *  \param c_Package Package class source.
     */
    
    Package(Package const& c_Package) noexcept;
    
    /**
     *  Default destructor.
     */

    ~Package() noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the package path.
     *
     *  \return The full package path.
     */
    
    std::string GetPackagePath() const noexcept;
    
    /**
     *  Get the package name.
     *
     *  \return The full package name.
     */
    
    std::string GetPackageName() const noexcept;

private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::string s_PackagePath;
    std::string s_PackageName;

protected:

};

#endif /* Package_h */
