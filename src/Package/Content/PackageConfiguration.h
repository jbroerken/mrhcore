/**
 *  PackageConfiguration.h
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

#ifndef PackageConfiguration_h
#define PackageConfiguration_h

// C / C++
#include <string>

// External
#include <MRH_Typedefs.h>

// Project
#include "../PackageException.h"


class PackageConfiguration
{
public:

    //*************************************************************************************
    // Types
    //*************************************************************************************

    typedef MRH_Uint32 EventPermission;
    
    typedef enum
    {
        CUSTOM = 0,
        APP = 1,
        LISTEN = 2,
        SAY = 3,
        PASSWORD = 4,
        USER = 5,
        NOTIFICATION = 6,
        
        EVENT_PERMISSION_LIST_MAX = NOTIFICATION,
        
        EVENT_PERMISSION_LIST_COUNT = EVENT_PERMISSION_LIST_MAX + 1
        
    }EventPermissionList;
    
    //*************************************************************************************
    // Getters
    //*************************************************************************************

    /**
     *  Get the app event version.
     *
     *  \return The app event version.
     */
    
    int GetAppEventVersion() const noexcept;
    
    /**
     *  Get the service event version.
     *
     *  \return The service event version.
     */
    
    int GetServiceEventVersion() const noexcept;
    
    /**
     *  Get a application package permissions. This function is thread safe.
     *
     *  \param e_Permission The permission to recieve.
     *
     *  \return The application package permissions.
     */
    
    EventPermission GetPermission(EventPermissionList e_Permission) const;

    /**
     *  Get the application package user id. This function is thread safe.
     *
     *  \return The application package user id.
     */
    
    int GetUserID() const noexcept;
    
    /**
     *  Get the application package group id. This function is thread safe.
     *
     *  \return The application package group id.
     */
    
    int GetGroupID() const noexcept;
    
    /**
     *  Check if the application package is a OS bundled application. This function is
     *  thread safe.
     *
     *  \return true if OS bundled, false if user installed.
     */
    
    bool GetOSApp() const noexcept;
    
    /**
     *  Get wether application stopping is disabled or not. This function is thread safe.
     *
     *  \return true if disabled, false if not.
     */
    
    bool GetStopDisabled() const noexcept;
    
    /**
     *  Check if the application service should be used. This function is thread safe.
     *
     *  \return true if usable, false if not.
     */
    
    bool GetUseAppService() const noexcept;
    
    /**
     *  Get the application service update timer. This function is thread safe.
     *
     *  \return The application service update timer in seconds.
     */
    
    MRH_Uint32 GetAppServiceUpdateTimerS() const noexcept;

    //*************************************************************************************
    // Data
    //*************************************************************************************
    
    static const EventPermission u32_NoPermission = 0;
    
private:

    //*************************************************************************************
    // Data
    //*************************************************************************************

    std::string s_FilePath;
    
    int i_AppEventVersion;
    int i_ServiceEventVersion;
    
    EventPermission p_Permission[EVENT_PERMISSION_LIST_COUNT];
    
    int i_UserID;
    int i_GroupID;
    
    bool b_OSApp;
    bool b_StopDisabled;
    
    bool b_UseAppService;
    MRH_Uint32 u32_AppServiceUpdateTimerS;
    
protected:

    //*************************************************************************************
    // Constructor / Destructor
    //*************************************************************************************

    /**
     *  Default constructor.
     *
     *  \param s_ConfigurationPath The full path to the application package configuration file.
     */

    PackageConfiguration(std::string const& s_ConfigurationPath);

    /**
     *  Copy constructor.
     *
     *  \param c_PackageConfiguration PackageConfiguration class source.
     */
    
    PackageConfiguration(PackageConfiguration const& c_PackageConfiguration) noexcept;
    
    /**
     *  Default destructor.
     */

    ~PackageConfiguration() noexcept;
};

#endif /* PackageConfiguration_h */
