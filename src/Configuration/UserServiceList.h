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
