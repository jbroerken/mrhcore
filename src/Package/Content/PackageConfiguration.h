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
    
    // NOTE: Bit flags are used (1, 2, 4, 8, etc.) to allow for combining types!
    typedef enum
    {
        NONE = 0, // Not an OS App
        UTILITY = 1, // General for OS apps which modify the OS or platform
        LAUNCHER = 2,
        SETTINGS = 4,
        PACKAGE_MANAGER = 8,
        
        OS_APP_TYPE_MAX = (PACKAGE_MANAGER * 2) - 1,
        
        OS_APP_TYPE_COUNT = 4
        
    }OSAppType;
    
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
     *  Get a application package permissions.
     *
     *  \param e_Permission The permission to recieve.
     *
     *  \return The application package permissions.
     */
    
    EventPermission GetPermission(EventPermissionList e_Permission) const;

    /**
     *  Get the application package user id.
     *
     *  \return The application package user id.
     */
    
    int GetUserID() const noexcept;
    
    /**
     *  Get the application package group id.
     *
     *  \return The application package group id.
     */
    
    int GetGroupID() const noexcept;
    
    /**
     *  Check if the application of the package is an OS application.
     *
     *  \return The OS app type.
     */
    
    OSAppType GetOSAppType() const noexcept;
    
    /**
     *  Get wether application stopping is disabled or not.
     *
     *  \return true if disabled, false if not.
     */
    
    bool GetStopDisabled() const noexcept;
    
    /**
     *  Check if the application service should be used.
     *
     *  \return true if usable, false if not.
     */
    
    bool GetUseAppService() const noexcept;
    
    /**
     *  Get the application service update timer.
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
    
    OSAppType e_OSAppType;
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
