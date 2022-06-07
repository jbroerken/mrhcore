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
