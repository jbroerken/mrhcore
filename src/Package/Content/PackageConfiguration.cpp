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

// C / C++
#include <cstring>

// External
#include <libmrhbf.h>

// Project
#include "./PackageConfiguration.h"
#include "../../Logger/Logger.h"

namespace
{
    enum Identifier
    {
        // Block Name
        BLOCK_EVENT_VERSION = 0,
        BLOCK_PERMISSIONS = 1,
        BLOCK_RUN_AS = 2,
        BLOCK_APP_SERVICE = 3,
        
        // Event Version Key
        KEY_EVENT_VERSION_APP = 4,
        KEY_EVENT_VERSION_SERVICE = 5,
        
        // Permissions Key
        KEY_PERMISSIONS_CUSTOM = 6,
        KEY_PERMISSIONS_APPLICATION = 7,
        KEY_PERMISSIONS_LISTEN,
        KEY_PERMISSIONS_SAY,
        KEY_PERMISSIONS_PASSWORD,
        KEY_PERMISSIONS_USER,
        
        // Run As Key
        KEY_RUN_AS_USER_ID,
        KEY_RUN_AS_GROUP_ID,
        KEY_RUN_AS_OS_APP_TYPE,
        KEY_RUN_AS_STOP_DISABLED,
        
        // App Service Key
        KEY_APP_SERVICE_USE,
        KEY_APP_SERVICE_UPDATE_TIMER,
        
        // Bounds
        IDENTIFIER_MAX = KEY_APP_SERVICE_UPDATE_TIMER,

        IDENTIFIER_COUNT = IDENTIFIER_MAX + 1
    };

    const char* p_Identifier[IDENTIFIER_COUNT] =
    {
        // Block Name
        "EventVersion",
        "Permissions",
        "RunAs",
        "AppService",
        
        // Event Version Key
        "App",
        "AppService",
        
        // Permissions Key
        "EventCustom",
        "EventApplication",
        "EventListen",
        "EventSay",
        "EventPassword",
        "EventUser",
        
        // Run As Key
        "UserID",
        "GroupID",
        "OSAppType",
        "StopDisabled",
        
        // App Service Key
        "UseAppService",
        "UpdateTimerS"
    };
}


//*************************************************************************************
// Constructor / Destructor
//*************************************************************************************/

// @NOTE: Pi is not a fan of [ 0 ... X ] = PackageConfiguration::u32_NoPermission
PackageConfiguration::PackageConfiguration(std::string const& s_ConfigurationPath) : p_Permission { PackageConfiguration::u32_NoPermission,
                                                                                                    PackageConfiguration::u32_NoPermission,
                                                                                                    PackageConfiguration::u32_NoPermission,
                                                                                                    PackageConfiguration::u32_NoPermission,
                                                                                                    PackageConfiguration::u32_NoPermission,
                                                                                                    PackageConfiguration::u32_NoPermission },
                                                                                     i_AppEventVersion(-1),
                                                                                     i_ServiceEventVersion(-1),
                                                                                     i_UserID(-1),
                                                                                     i_GroupID(-1),
                                                                                     e_OSAppType(NONE),
                                                                                     b_StopDisabled(false),
                                                                                     b_UseAppService(false),
                                                                                     u32_AppServiceUpdateTimerS(0)
{
    try
    {
        MRH_BlockFile c_File((this->s_FilePath = s_ConfigurationPath));
        
        for (auto& Block : c_File.l_Block)
        {
            std::string s_Name(Block.GetName());
            
            if (s_Name.compare(p_Identifier[BLOCK_EVENT_VERSION]) == 0)
            {
                i_AppEventVersion = std::stoi(Block.GetValue(p_Identifier[KEY_EVENT_VERSION_APP]));
                i_ServiceEventVersion = std::stoi(Block.GetValue(p_Identifier[KEY_EVENT_VERSION_SERVICE]));
            }
            else if (s_Name.compare(p_Identifier[BLOCK_PERMISSIONS]) == 0)
            {
                // Special catch: Missing / wrong permissions can be assumed as 0
                try
                {
                    p_Permission[CUSTOM] = static_cast<EventPermission>(std::stoull(Block.GetValue(p_Identifier[KEY_PERMISSIONS_CUSTOM])));
                    p_Permission[APP] = static_cast<EventPermission>(std::stoull(Block.GetValue(p_Identifier[KEY_PERMISSIONS_APPLICATION])));
                    p_Permission[LISTEN] = static_cast<EventPermission>(std::stoull(Block.GetValue(p_Identifier[KEY_PERMISSIONS_LISTEN])));
                    p_Permission[SAY] = static_cast<EventPermission>(std::stoull(Block.GetValue(p_Identifier[KEY_PERMISSIONS_SAY])));
                    p_Permission[PASSWORD] = static_cast<EventPermission>(std::stoull(Block.GetValue(p_Identifier[KEY_PERMISSIONS_PASSWORD])));
                    p_Permission[USER] = static_cast<EventPermission>(std::stoull(Block.GetValue(p_Identifier[KEY_PERMISSIONS_USER])));
                }
                catch (std::exception& e) // + MRH_BFException
                {
                    Logger::Singleton().Log(Logger::WARNING, "Failed to read package permissions for file " +
                                                             s_FilePath +
                                                             ": " +
                                                             e.what(),
                                            "PackageConfiguration.cpp", __LINE__);
                }
            }
            else if (s_Name.compare(p_Identifier[BLOCK_RUN_AS]) == 0)
            {
                i_UserID = std::stoi(Block.GetValue(p_Identifier[KEY_RUN_AS_USER_ID]));
                i_GroupID = std::stoi(Block.GetValue(p_Identifier[KEY_RUN_AS_GROUP_ID]));
                
                int i_OSAppType = std::stoi(Block.GetValue(p_Identifier[KEY_RUN_AS_OS_APP_TYPE]));
                e_OSAppType = i_OSAppType <= NONE ? NONE : static_cast<OSAppType>(i_OSAppType);
                
                b_StopDisabled = Block.GetValue(p_Identifier[KEY_RUN_AS_STOP_DISABLED]).compare("1") == 0 ? true : false;
            }
            else if (s_Name.compare(p_Identifier[BLOCK_APP_SERVICE]) == 0)
            {
                b_UseAppService = Block.GetValue(p_Identifier[KEY_APP_SERVICE_USE]).compare("1") == 0 ? true : false;
                u32_AppServiceUpdateTimerS = static_cast<MRH_Uint32>(std::stoull(Block.GetValue(p_Identifier[KEY_APP_SERVICE_UPDATE_TIMER])));
            }
        }
    }
    catch (std::exception& e) // + MRH_BFException
    {
        throw PackageException("Could not read package configuration (" + std::string(e.what()) + ")!", s_FilePath);
    }
}

PackageConfiguration::PackageConfiguration(PackageConfiguration const& c_PackageConfiguration) noexcept
{
    s_FilePath = c_PackageConfiguration.s_FilePath;
    
    i_AppEventVersion = c_PackageConfiguration.i_AppEventVersion;
    i_ServiceEventVersion = c_PackageConfiguration.i_ServiceEventVersion;
    
    std::memcpy(p_Permission, c_PackageConfiguration.p_Permission, sizeof(EventPermission) * EVENT_PERMISSION_LIST_COUNT);
    
    i_UserID = c_PackageConfiguration.i_UserID;
    i_GroupID = c_PackageConfiguration.i_GroupID;
    
    e_OSAppType = c_PackageConfiguration.e_OSAppType;
    b_StopDisabled = c_PackageConfiguration.b_StopDisabled;
    
    b_UseAppService = c_PackageConfiguration.b_UseAppService;
    u32_AppServiceUpdateTimerS = c_PackageConfiguration.u32_AppServiceUpdateTimerS;
}

PackageConfiguration::~PackageConfiguration() noexcept
{}

//*************************************************************************************
// Getters
//*************************************************************************************

int PackageConfiguration::GetAppEventVersion() const noexcept
{
    return i_AppEventVersion;
}

int PackageConfiguration::GetServiceEventVersion() const noexcept
{
    return i_ServiceEventVersion;
}

PackageConfiguration::EventPermission PackageConfiguration::GetPermission(EventPermissionList e_Permission) const
{
    if (e_Permission < 0 || e_Permission > EVENT_PERMISSION_LIST_MAX)
    {
        throw PackageException("Invalid event permission requested: " + std::to_string(e_Permission), s_FilePath);
    }
    
    return p_Permission[e_Permission];
}

int PackageConfiguration::GetUserID() const noexcept
{
    return i_UserID;
}

int PackageConfiguration::GetGroupID() const noexcept
{
    return i_GroupID;
}

PackageConfiguration::OSAppType PackageConfiguration::GetOSAppType() const noexcept
{
    return e_OSAppType;
}

bool PackageConfiguration::GetStopDisabled() const noexcept
{
    return b_StopDisabled;
}

bool PackageConfiguration::GetUseAppService() const noexcept
{
    return b_UseAppService;
}

MRH_Uint32 PackageConfiguration::GetAppServiceUpdateTimerS() const noexcept
{
    return u32_AppServiceUpdateTimerS;
}
