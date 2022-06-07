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
