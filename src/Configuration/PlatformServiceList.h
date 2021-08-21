/**
 *  PlatformServiceList.h
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
