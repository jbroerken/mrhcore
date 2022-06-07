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

#ifndef PlatformServiceList_h
#define PlatformServiceList_h

// C / C++
#include <vector>

// External
#include <MRH_Typedefs.h>

// Project
#include "./ConfigurationException.h"


class PlatformServiceList
{
public:

    //*************************************************************************************
    // Service Info
    //*************************************************************************************

    class Service
    {
        friend class PlatformServiceList;
        
    public:
        
        //*************************************************************************************
        // Constructor / Destructor
        //*************************************************************************************

        /**
         *  Copy constructor. Disabled for this class.
         *
         *  \param c_Service Service class source.
         */

        Service(Service const& c_Service) noexcept;
        
        /**
         *  Default destructor.
         */

        ~Service() noexcept;

        //*************************************************************************************
        // Data
        //*************************************************************************************

        std::string s_BinaryPath;
        MRH_Uint32 u32_RouteID;
        bool b_Disabled;
        bool b_Essential;
        
    private:
        
        //*************************************************************************************
        // Constructor
        //*************************************************************************************

        /**
         *  Default constructor.
         *
         *  \param s_BinaryPath The full path to the platform service binary.
         *  \param u32_RouteID The service event route id.
         *  \param b_Disabled Defines if the service is disabled or not.
         *  \param b_Essential Defines if the service is essential to exist or not.
         */

        Service(std::string const& s_BinaryPath,
                MRH_Uint32 u32_RouteID,
                bool b_Disabled,
                bool b_Essential) noexcept;

        //*************************************************************************************
        // Data
        //*************************************************************************************
        
    protected:
        
    };
    
    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     */

    PlatformServiceList();

    /**
     *  Copy constructor. Disabled for this class.
     *
     *  \param c_PlatformServiceList PlatformServiceList class source.
     */

    PlatformServiceList(PlatformServiceList const& c_PlatformServiceList) = delete;

    /**
     *  Default destructor.
     */

    ~PlatformServiceList() noexcept;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the amount of user services in this list.
     *
     *  \return The platform service count.
     */

    size_t GetServiceCount() const noexcept;
    
    /**
     *  Get a platform service.
     *
     *  \param us_Service The platform service requested.
     *
     *  \return The requested platform service.
     */
    
    Service const& GetService(size_t us_Service) const;
    
private:
    
    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::vector<Service> v_Service;
    
protected:

};

#endif /* PlatformServiceList_h */
