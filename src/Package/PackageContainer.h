/**
 *  PackageContainer.h
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

#ifndef PackageContainer_h
#define PackageContainer_h

// C / C++
#include <vector>
#include <mutex>

// External

// Project
#include "./Package.h"


class PackageContainer
{
public:
    
    //*************************************************************************************
    // Constructor
    //*************************************************************************************

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PackageContainer PackageContainer class source.
     */

    PackageContainer(PackageContainer const& c_PackageContainer) = delete;

    //*************************************************************************************
    // Singleton
    //*************************************************************************************

    /**
     *  Get the class instance. This function is thread safe.
     *
     *  \return The class instance.
     */

    static PackageContainer& Singleton() noexcept;

    //*************************************************************************************
    // Reload
    //*************************************************************************************

    /**
     *  Reload the package container. This function is thread safe.
     */

    void Reload() noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the number of packages in this container. This function is thread safe.
     *
     *  \return The number of packages in this list.
     */
    
    size_t GetPackageCount() noexcept;
    
    /**
     *  Get a package by index. This function is thread safe.
     *
     *  \param us_Package The package to get.
     *
     *  \return The requested package.
     */
    
    Package const& GetPackage(size_t us_Package);
    
    /**
     *  Get a package by package name. This function is thread safe.
     *
     *  \param s_PackagePath The full path to the package.
     *
     *  \return The requested package.
     */
    
    Package const& GetPackage(std::string s_PackagePath);
    
    /**
     *  Check if a package exists. This function is thread safe.
     *
     *  \param s_PackagePath The full package path.
     *
     *  \return true if the package exists, false if not.
     */
    
    bool GetPackageExists(std::string s_PackagePath) noexcept;

private:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    PackageContainer() noexcept;

    /**
     *  Default destructor.
     */

    ~PackageContainer() noexcept;

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::mutex c_Mutex;
    std::vector<Package> v_Package;

protected:

};

#endif /* PackageContainer_h */
