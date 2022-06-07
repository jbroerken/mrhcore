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
