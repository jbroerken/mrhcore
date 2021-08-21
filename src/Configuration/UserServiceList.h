/**
 *  UserServiceList.h
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

#ifndef UserServiceList_h
#define UserServiceList_h

// C / C++
#include <vector>

// External

// Project
#include "./ConfigurationException.h"


class UserServiceList
{
public:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    UserServiceList();

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_UserServiceList UserServiceList class source.
     */

    UserServiceList(UserServiceList const& c_UserServiceList) = delete;

    /**
     *  Default destructor.
     */

    ~UserServiceList() noexcept;

    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the amount of user services packages in this list.
     *
     *  \return The user service package count.
     */

    size_t GetPackageCount() const noexcept;
    
    /**
     *  Get a user service package.
     *
     *  \param us_Package The user service package requested.
     *
     *  \return The requested user service package.
     */
    
    std::string GetPackage(size_t us_Package) const;
    
    /**
     *  Check if a package is a listed user service.
     *
     *  \param s_Package The full path to the user service package.
     *
     *  \return true if the package contains a user service listed, false if not.
     */
    
    bool GetUserService(std::string s_Package) const noexcept;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::vector<std::string> v_Package;
    
protected:

};

#endif /* UserServiceList_h */
