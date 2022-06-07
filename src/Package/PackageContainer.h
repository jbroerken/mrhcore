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
