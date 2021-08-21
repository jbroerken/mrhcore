/**
 *  PackageLocationList.h
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

#ifndef PackageLocationList_h
#define PackageLocationList_h

// C / C++
#include <vector>

// External

// Project
#include "./ConfigurationException.h"


class PackageList
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    PackageList();

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PackageList PackageList class source.
     */

    PackageList(PackageList const& c_PackageList) = delete;

    /**
     *  Default destructor.
     */

    ~PackageList() noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************
    
    /**
     *  Get all package paths.
     *
     *  \return The package paths.
     */
    
    std::vector<std::string> GetPackages() const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::vector<std::string> v_Package;
    
protected:

};

#endif /* PackageLocationList_h */
